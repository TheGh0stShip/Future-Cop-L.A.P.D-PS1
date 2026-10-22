#!/usr/bin/env python3
"""Compile candidate units across compiler lanes and rank byte similarity."""

from __future__ import annotations

import argparse
import json
import tempfile
from pathlib import Path

try:
    import verify_matching_units as verifier
except ModuleNotFoundError:  # Imported as tools.sweep_candidate_compilers.
    from tools import verify_matching_units as verifier


ROOT = Path(__file__).resolve().parents[1]
HEADER_SIZE = 0x800
LOAD_ADDRESS = 0x80010000
DEFAULT_COMPILERS = ("gcc-2.8.1-psx", "gcc-2.7.2-psx")


def compare(expected: bytes, actual: bytes) -> dict:
    shared = min(len(expected), len(actual))
    mismatch = next(
        (offset for offset in range(shared) if expected[offset] != actual[offset]),
        None,
    )
    if mismatch is None and len(actual) < len(expected):
        mismatch = len(actual)
    matching_bytes = sum(
        left == right for left, right in zip(expected, actual[:len(expected)])
    )
    matching_words = sum(
        expected[offset:offset + 4] == actual[offset:offset + 4]
        for offset in range(0, len(expected), 4)
        if len(actual[offset:offset + 4]) == 4
    )
    nonzero_tail = sum(byte != 0 for byte in actual[len(expected):])
    exact = actual[:len(expected)] == expected and nonzero_tail == 0
    result = {
        "exact": exact,
        "expected_size": len(expected),
        "actual_size": len(actual),
        "matching_bytes": matching_bytes,
        "matching_words": matching_words,
        "nonzero_tail_bytes": nonzero_tail,
        "first_mismatch": mismatch,
    }
    if mismatch is not None:
        word_offset = mismatch & ~3
        result["mismatch_word_offset"] = word_offset
        result["expected_word"] = expected[word_offset:word_offset + 4].hex()
        result["actual_word"] = actual[word_offset:word_offset + 4].hex()
    return result


def score(result: dict) -> tuple[int, int, int, int]:
    prefix = result["first_mismatch"]
    if prefix is None:
        prefix = result["expected_size"]
    return (
        int(result["exact"]),
        result["matching_words"],
        result["matching_bytes"],
        prefix,
    )


def sweep(config_path: Path, executable_path: Path, compilers: list[str],
          prefix: str, unit_names: set[str] | None = None) -> dict:
    config = json.loads(config_path.read_text())
    verifier.validate(config)
    if unit_names:
        available = {unit["name"] for unit in config["units"]}
        missing = sorted(unit_names - available)
        if missing:
            raise ValueError("unknown candidate unit(s): " + ", ".join(missing))
        config["units"] = [
            unit for unit in config["units"] if unit["name"] in unit_names
        ]
    executable = executable_path.read_bytes()
    units = []
    with tempfile.TemporaryDirectory(prefix="future-cop-sweep-") as directory:
        root = Path(directory)
        for unit_index, original in enumerate(config["units"]):
            address = verifier.number(original["address"])
            size = verifier.number(original["size"])
            offset = HEADER_SIZE + address - LOAD_ADDRESS
            expected = executable[offset:offset + size]
            if len(expected) != size:
                raise ValueError(f"{original['name']}: range outside executable")
            variants = []
            for compiler_index, compiler in enumerate(compilers):
                # Candidate bytes must be evaluated at their retail address.
                # Otherwise local MIPS J/JAL targets retain section-relative
                # encodings and make byte-identical C appear mismatched.
                unit = {**original, "compiler": compiler, "link": True}
                work = root / f"{unit_index}-{compiler_index}"
                work.mkdir()
                try:
                    actual = verifier.compile_unit(unit, config, prefix, work)
                    result = compare(expected, actual)
                    result["compiler"] = compiler
                except Exception as error:  # Preserve failures and continue the sweep.
                    result = {
                        "compiler": compiler,
                        "exact": False,
                        "error": f"{type(error).__name__}: {error}",
                    }
                variants.append(result)
            successful = [item for item in variants if "error" not in item]
            best = max(successful, key=score) if successful else variants[0]
            units.append({
                "name": original["name"],
                "address": original["address"],
                "size": original["size"],
                "source": original["source"],
                "best_compiler": best["compiler"],
                "exact": best["exact"],
                "variants": variants,
            })
    exact = [unit for unit in units if unit["exact"]]
    return {
        "schema": 1,
        "config": str(config_path),
        "compilers": compilers,
        "linked_at_retail_address": True,
        "candidate_functions": len(units),
        "candidate_bytes": sum(verifier.number(unit["size"]) for unit in units),
        "exact_functions": len(exact),
        "exact_bytes": sum(verifier.number(unit["size"]) for unit in exact),
        "units": units,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", type=Path,
                        default=ROOT / "config/us/candidate_units.json")
    parser.add_argument("--executable", type=Path,
                        default=ROOT / "baseroms/us/SLUS_007.39")
    parser.add_argument("--output", type=Path,
                        default=ROOT / "build/us/compiler_sweep.json")
    parser.add_argument("--compiler", action="append", dest="compilers")
    parser.add_argument(
        "--unit", action="append", dest="units",
        help="sweep only this candidate name; may be repeated",
    )
    parser.add_argument("--binutils-prefix", default="mipsel-linux-gnu-")
    args = parser.parse_args()
    report = sweep(
        args.config,
        args.executable,
        args.compilers or list(DEFAULT_COMPILERS),
        args.binutils_prefix,
        set(args.units) if args.units else None,
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(
        f"wrote {args.output}: {report['exact_functions']} exact functions / "
        f"{report['exact_bytes']} bytes from {report['candidate_functions']} candidates"
    )


if __name__ == "__main__":
    main()
