#!/usr/bin/env python3
"""Run resumable, compiler-guided C reconstruction jobs on local hardware."""

from __future__ import annotations

import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
import threading
from dataclasses import dataclass
from pathlib import Path
from typing import Any

import verify_matching_units as matching


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_ASSEMBLY = ROOT / "asm/us/main.s"
DEFAULT_EXECUTABLE = ROOT / "baseroms/us/SLUS_007.39"
DEFAULT_OUTPUT = ROOT / "build/us/offline-decomp"
MATCHING_MANIFEST = ROOT / "config/us/matching_units.json"
CANDIDATE_MANIFEST = ROOT / "config/us/candidate_units.json"
FUNCTION_HEADER = re.compile(
    r"^nonmatching\s+(?P<name>func_[0-9A-Fa-f]{8}|entrypoint),\s*"
    r"(?P<size>0x[0-9A-Fa-f]+)\s*$"
)
GLOBAL_SYMBOL = re.compile(r"\b(?:func|D)_[0-9A-Fa-f]{8}\b")
SAFE_SYMBOL = re.compile(r"^(?:func|D)_[0-9A-Fa-f]{8}$")
NON_C_SOURCE = re.compile(
    r"(?:\b__asm__\b|\b(?:asm|__asm)\s*\(|^\s*\.word\b|^\s*\.include\b)",
    re.MULTILINE,
)


@dataclass(frozen=True)
class Target:
    name: str
    address: int
    size: int
    assembly: str


def atomic_json(path: Path, value: Any) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")
    temporary.replace(path)


def load_json(path: Path, default: Any) -> Any:
    return json.loads(path.read_text()) if path.is_file() else default


def parse_targets(text: str) -> list[Target]:
    """Read Splat function blocks without treating their labels as proof."""
    lines = text.splitlines()
    targets: list[Target] = []
    index = 0
    while index < len(lines):
        match = FUNCTION_HEADER.match(lines[index].strip())
        if not match:
            index += 1
            continue
        name = match.group("name")
        size = int(match.group("size"), 0)
        address = 0x80096520 if name == "entrypoint" else int(name[5:], 16)
        start = index
        index += 1
        terminator = f"endlabel {name}"
        while index < len(lines) and lines[index].strip() != terminator:
            index += 1
        if index == len(lines):
            raise ValueError(f"{name}: missing {terminator}")
        index += 1
        targets.append(Target(name, address, size, "\n".join(lines[start:index]).strip()))
    return targets


def inferred_symbols(target: Target) -> dict[str, str]:
    symbols = {}
    for name in sorted(set(GLOBAL_SYMBOL.findall(target.assembly)) - {target.name}):
        symbols[name] = f"0x{int(name.rsplit('_', 1)[1], 16):08X}"
    return symbols


def score_bytes(actual: bytes, expected: bytes) -> dict[str, Any]:
    padded = actual[:len(expected)] + bytes(max(0, len(expected) - len(actual)))
    equal_bytes = sum(left == right for left, right in zip(padded, expected))
    words = len(expected) // 4
    equal_words = sum(
        padded[offset:offset + 4] == expected[offset:offset + 4]
        for offset in range(0, words * 4, 4)
    )
    prefix = 0
    for left, right in zip(padded, expected):
        if left != right:
            break
        prefix += 1
    trailing = len(actual) > len(expected) and any(actual[len(expected):])
    mismatches = []
    for offset in range(0, len(expected), 4):
        expected_word = expected[offset:offset + 4]
        actual_word = padded[offset:offset + 4]
        if actual_word != expected_word:
            mismatches.append({
                "offset": f"0x{offset:X}",
                "expected_le": expected_word.hex().upper(),
                "actual_le": actual_word.hex().upper(),
            })
    return {
        "exact": padded == expected and not trailing,
        "equal_bytes": equal_bytes,
        "total_bytes": len(expected),
        "equal_words": equal_words,
        "total_words": words,
        "matching_prefix_bytes": prefix,
        "nonzero_bytes_after_extent": trailing,
        "compiled_size": len(actual),
        "mismatches": mismatches[:64],
        "mismatch_list_truncated": len(mismatches) > 64,
        "output_sha256": hashlib.sha256(actual).hexdigest(),
    }


def decode_backend_output(text: str) -> tuple[str, dict[str, str], str]:
    """Validate the deliberately narrow proposal-command response."""
    value = json.loads(text)
    if not isinstance(value, dict) or not isinstance(value.get("source"), str):
        raise ValueError("response must be an object containing a source string")
    source = value["source"].strip().replace(
        '#include "../../include/', '#include "'
    ) + "\n"
    if not source.strip():
        raise ValueError("source must not be empty")
    if NON_C_SOURCE.search(source):
        raise ValueError("source contains assembly; reconstructed C is required")
    raw_symbols = value.get("symbols", {})
    if not isinstance(raw_symbols, dict):
        raise ValueError("symbols must be an object")
    symbols = {}
    for name, address in raw_symbols.items():
        if not isinstance(name, str) or not SAFE_SYMBOL.fullmatch(name):
            raise ValueError(f"unsafe symbol name: {name!r}")
        parsed = int(address, 0) if isinstance(address, str) else int(address)
        if not 0 <= parsed <= 0xFFFFFFFF:
            raise ValueError(f"symbol address outside 32-bit range: {name}")
        symbols[name] = f"0x{parsed:08X}"
    note = value.get("note", "")
    if not isinstance(note, str):
        raise ValueError("note must be a string")
    return source, symbols, note


def compiler_environment() -> dict[str, str]:
    environment = os.environ.copy()
    bin_dir = ROOT / ".toolchain/mipsel-binutils/root/usr/bin"
    lib_dir = ROOT / ".toolchain/mipsel-binutils/root/usr/lib/x86_64-linux-gnu"
    environment["PATH"] = f"{bin_dir}{os.pathsep}{environment.get('PATH', '')}"
    current = environment.get("LD_LIBRARY_PATH", "")
    environment["LD_LIBRARY_PATH"] = str(lib_dir) + (os.pathsep + current if current else "")
    return environment


class Runner:
    def __init__(self, args: argparse.Namespace):
        self.args = args
        self.output = args.output.resolve()
        try:
            self.output.relative_to((ROOT / "build").resolve())
        except ValueError as error:
            raise ValueError("worker output must remain beneath the ignored build directory") from error
        self.executable = args.executable.read_bytes()
        self.config = load_json(MATCHING_MANIFEST, {})
        self.candidate_config = load_json(CANDIDATE_MANIFEST, {"units": []})
        self.command = shlex.split(args.backend_command)
        if not self.command:
            raise ValueError("proposal command is empty")
        self.print_lock = threading.Lock()

    def expected(self, target: Target) -> bytes:
        offset = matching.HEADER_SIZE + target.address - matching.LOAD_ADDRESS
        result = self.executable[offset:offset + target.size]
        if len(result) != target.size:
            raise ValueError(f"{target.name}: range is outside the executable")
        return result

    def initial_source(self, target: Target) -> str | None:
        for unit in self.candidate_config.get("units", []):
            if unit.get("name") == target.name:
                path = ROOT / unit["source"]
                return path.read_text() if path.is_file() else None
        return None

    def request(self, target: Target, state: dict[str, Any]) -> dict[str, Any]:
        best = state.get("best")
        return {
            "protocol": 1,
            "task": "reconstruct_byte_matching_c",
            "target": {
                "name": target.name,
                "address": f"0x{target.address:08X}",
                "size": target.size,
                "architecture": "MIPS R3000A, little-endian, 32-bit",
                "compiler": self.config["compiler"],
                "compiler_flags": self.config["compiler_flags"],
                "assembly_oracle": target.assembly,
                "known_symbols": inferred_symbols(target),
            },
            "attempt": state.get("attempts", 0) + 1,
            "best_candidate": None if best is None else {
                "source": (self.output / best["source"]).read_text(),
                "symbols": best["symbols"],
                "score": best["score"],
                "compiler_diagnostic": best.get("compiler_diagnostic", ""),
            },
            "requirements": [
                "Return reconstructed C source, not assembly.",
                "Use ANSI C accepted by the pinned compiler.",
                "Use project includes such as #include \"game/types.h\" when needed.",
                "Preserve 32-bit PS1 address and integer semantics.",
                "Return only one JSON object: source (string), symbols (object), note (string).",
            ],
        }

    def call_backend(self, payload: dict[str, Any]) -> tuple[str, dict[str, str], str]:
        completed = subprocess.run(
            self.command,
            input=json.dumps(payload),
            text=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            timeout=self.args.timeout,
            cwd=ROOT,
            env=compiler_environment(),
            check=False,
        )
        if completed.returncode:
            detail = completed.stderr.strip()[-2000:]
            raise RuntimeError(f"proposal command exited {completed.returncode}: {detail}")
        return decode_backend_output(completed.stdout)

    def compile(self, target: Target, source_path: Path,
                symbols: dict[str, str]) -> tuple[bytes | None, str]:
        relative = source_path.relative_to(ROOT)
        unit = {
            "name": target.name,
            "address": target.address,
            "size": target.size,
            "source": str(relative),
            "symbols": inferred_symbols(target) | symbols,
        }
        try:
            with tempfile.TemporaryDirectory(prefix="future-cop-worker-") as directory:
                actual = matching.compile_unit(
                    unit, self.config, self.args.binutils_prefix, Path(directory),
                    environment=compiler_environment(),
                )
            return actual, ""
        except (subprocess.CalledProcessError, OSError, ValueError) as error:
            return None, str(error)

    @staticmethod
    def failed_score(expected: bytes) -> dict[str, Any]:
        score = score_bytes(b"", expected)
        score["exact"] = False
        score["compile_failed"] = True
        return score

    @staticmethod
    def better(score: dict[str, Any], best: dict[str, Any] | None) -> bool:
        if best is None:
            return True
        previous = best["score"]
        return (score["exact"], score["equal_words"], score["equal_bytes"],
                score["matching_prefix_bytes"], -score["compiled_size"]) > (
                    previous["exact"], previous["equal_words"], previous["equal_bytes"],
                    previous["matching_prefix_bytes"], -previous["compiled_size"]
                )

    def run_target(self, target: Target) -> dict[str, Any]:
        target_dir = self.output / "work" / target.name
        state_path = target_dir / "state.json"
        state = load_json(state_path, {
            "schema": 1,
            "name": target.name,
            "address": f"0x{target.address:08X}",
            "size": target.size,
            "attempts": 0,
            "status": "pending",
        })
        if state.get("status") == "exact":
            return state

        initial = self.initial_source(target)
        if initial is not None and state["attempts"] == 0 and "best" not in state:
            seed_path = target_dir / "seed.c"
            seed_path.parent.mkdir(parents=True, exist_ok=True)
            seed_path.write_text(initial)
            actual, diagnostic = self.compile(target, seed_path, {})
            score = (score_bytes(actual, self.expected(target)) if actual is not None
                     else self.failed_score(self.expected(target)))
            state["best"] = {
                "source": str(seed_path.relative_to(self.output)),
                "symbols": {},
                "score": score,
                "compiler_diagnostic": diagnostic,
            }
            atomic_json(state_path, state)

        while state["attempts"] < self.args.attempts and state.get("status") != "exact":
            attempt = state["attempts"] + 1
            attempt_dir = target_dir / "attempts" / f"{attempt:06d}"
            attempt_dir.mkdir(parents=True, exist_ok=True)
            try:
                source, symbols, note = self.call_backend(self.request(target, state))
                source_hash = hashlib.sha256(source.encode()).hexdigest()
                seen_sources = set(state.get("seen_source_sha256", []))
                if source_hash in seen_sources:
                    raise ValueError("proposal repeated an already evaluated source")
                seen_sources.add(source_hash)
                state["seen_source_sha256"] = sorted(seen_sources)
                source_path = attempt_dir / "candidate.c"
                source_path.write_text(source)
                actual, diagnostic = self.compile(target, source_path, symbols)
                score = (score_bytes(actual, self.expected(target)) if actual is not None
                         else self.failed_score(self.expected(target)))
                result = {"score": score, "symbols": symbols, "note": note,
                          "compiler_diagnostic": diagnostic,
                          "source_sha256": source_hash}
                atomic_json(attempt_dir / "result.json", result)
                if self.better(score, state.get("best")):
                    state["best"] = {
                        "source": str(source_path.relative_to(self.output)),
                        "symbols": symbols,
                        "score": score,
                        "compiler_diagnostic": diagnostic,
                    }
                if score["exact"]:
                    inbox = self.output / "inbox" / target.name
                    inbox.mkdir(parents=True, exist_ok=True)
                    (inbox / "candidate.c").write_text(source)
                    atomic_json(inbox / "unit.json", {
                        "name": target.name,
                        "address": f"0x{target.address:08X}",
                        "size": f"0x{target.size:X}",
                        "source": "candidate.c",
                        "symbols": inferred_symbols(target) | symbols,
                        "source_sha256": hashlib.sha256(source.encode()).hexdigest(),
                        "matched": True,
                        "semantic_review_required": True,
                    })
                    state["status"] = "exact"
            except (json.JSONDecodeError, subprocess.TimeoutExpired, RuntimeError,
                    ValueError) as error:
                atomic_json(attempt_dir / "result.json", {"error": str(error)})
                state["last_error"] = str(error)
            state["attempts"] = attempt
            atomic_json(state_path, state)
            with self.print_lock:
                best = state.get("best", {}).get("score", {})
                print(f"{target.name} attempt {attempt}: {state.get('status')} "
                      f"words={best.get('equal_words', 0)}/{best.get('total_words', 0)}",
                      flush=True)
        return state


def matched_names() -> set[str]:
    return {unit["name"] for unit in load_json(MATCHING_MANIFEST, {"units": []})["units"]}


def select_targets(args: argparse.Namespace) -> list[Target]:
    targets = parse_targets(args.assembly.read_text())
    completed = matched_names()
    requested = set(args.function or [])
    selected = [target for target in targets
                if target.name not in completed
                and args.min_size <= target.size <= args.max_size
                and (not requested or target.name in requested)]
    selected.sort(key=lambda target: (target.size, target.address))
    if requested - {target.name for target in selected}:
        missing = ", ".join(sorted(requested - {target.name for target in selected}))
        raise SystemExit(f"requested targets are unavailable or outside size limits: {missing}")
    return selected[:args.targets]


def run_command(args: argparse.Namespace) -> int:
    targets = select_targets(args)
    if not targets:
        print("No eligible unmatched targets.")
        return 0
    runner = Runner(args)
    print(f"Running {len(targets)} target(s), {args.jobs} parallel job(s), "
          f"up to {args.attempts} cumulative attempt(s) per target.")
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as executor:
        states = list(executor.map(runner.run_target, targets))
    exact = sum(state.get("status") == "exact" for state in states)
    print(f"Run complete: {exact} exact match(es); review {runner.output / 'inbox'}")
    return 0


def status_command(args: argparse.Namespace) -> int:
    states = []
    for path in sorted((args.output / "work").glob("*/state.json")):
        states.append(load_json(path, {}))
    exact = [state for state in states if state.get("status") == "exact"]
    attempts = sum(state.get("attempts", 0) for state in states)
    print(f"targets={len(states)} exact={len(exact)} attempts={attempts}")
    for state in exact:
        print(f"EXACT {state['name']} {state['size']} bytes")
    return 0


def parser() -> argparse.ArgumentParser:
    result = argparse.ArgumentParser(description=__doc__)
    subparsers = result.add_subparsers(dest="action", required=True)
    run_parser = subparsers.add_parser("run", help="run or resume reconstruction jobs")
    run_parser.add_argument("--backend-command", required=True,
                            help="local JSON proposal command, parsed like a shell command")
    run_parser.add_argument("--jobs", type=int, default=max(1, (os.cpu_count() or 2) // 2))
    run_parser.add_argument("--attempts", type=int, default=50,
                            help="cumulative attempt ceiling for each target")
    run_parser.add_argument("--targets", type=int, default=16)
    run_parser.add_argument("--min-size", type=lambda value: int(value, 0), default=8)
    run_parser.add_argument("--max-size", type=lambda value: int(value, 0), default=0x80)
    run_parser.add_argument("--function", action="append",
                            help="restrict work to a function; may be repeated")
    run_parser.add_argument("--timeout", type=int, default=600)
    run_parser.add_argument("--assembly", type=Path, default=DEFAULT_ASSEMBLY)
    run_parser.add_argument("--executable", type=Path, default=DEFAULT_EXECUTABLE)
    run_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    run_parser.add_argument("--binutils-prefix", default=os.environ.get(
        "MIPSEL_PREFIX", "mipsel-linux-gnu-"))
    run_parser.set_defaults(handler=run_command)
    status_parser = subparsers.add_parser("status", help="summarize saved worker state")
    status_parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    status_parser.set_defaults(handler=status_command)
    return result


if __name__ == "__main__":
    arguments = parser().parse_args()
    if getattr(arguments, "jobs", 1) < 1 or getattr(arguments, "attempts", 1) < 1:
        raise SystemExit("jobs and attempts must be positive")
    sys.exit(arguments.handler(arguments))
