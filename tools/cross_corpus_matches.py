#!/usr/bin/env python3
"""Find exact and relocation-tolerant function matches in a PS1 code corpus."""

from __future__ import annotations

import argparse
import json
import struct
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_LOAD_ADDRESS = 0x80010000


def number(value: str | int) -> int:
    return int(value, 0) if isinstance(value, str) else value


def normalize_jump_targets(data: bytes) -> bytes:
    """Mask only the destination field of aligned MIPS J and JAL words."""
    output = bytearray(data)
    for offset in range(0, len(output) - 3, 4):
        word = struct.unpack_from("<I", output, offset)[0]
        if word >> 26 in (2, 3):
            struct.pack_into("<I", output, offset, word & 0xFC000000)
    return bytes(output)


def aligned_occurrences(haystack: bytes, needle: bytes) -> list[int]:
    positions = []
    start = 0
    while True:
        position = haystack.find(needle, start)
        if position < 0:
            return positions
        if position % 4 == 0:
            positions.append(position)
        start = position + 1


def scan(target: bytes, functions: list[dict], corpus: bytes,
         target_load: int, corpus_load: int, minimum_size: int,
         maximum_size: int) -> dict:
    normalized_target = normalize_jump_targets(target)
    normalized_corpus = normalize_jump_targets(corpus)
    exact = []
    normalized = []
    scanned = 0
    for function in functions:
        if function.get("is_data") or function.get("alias_of"):
            continue
        address = number(function["addr"])
        size = number(function["size"])
        if not minimum_size <= size <= maximum_size:
            continue
        offset = address - target_load
        if offset < 0 or offset + size > len(target):
            continue
        scanned += 1
        blob = target[offset:offset + size]
        normalized_blob = normalized_target[offset:offset + size]
        exact_positions = set(aligned_occurrences(corpus, blob))
        for position in sorted(exact_positions):
            exact.append({
                "function": function["name"],
                "target_address": address,
                "size": size,
                "corpus_address": corpus_load + position,
            })
        for position in aligned_occurrences(normalized_corpus, normalized_blob):
            if position not in exact_positions:
                normalized.append({
                    "function": function["name"],
                    "target_address": address,
                    "size": size,
                    "corpus_address": corpus_load + position,
                })
    return {
        "functions_scanned": scanned,
        "exact_hits": exact,
        "jump_normalized_hits": normalized,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--target", type=Path,
                        default=ROOT / "baseroms/us/SLUS_007.39")
    parser.add_argument("--analysis", type=Path,
                        default=ROOT / "analysis/analysis.json")
    parser.add_argument("--target-header", type=number, default=0x800)
    parser.add_argument("--target-load", type=number,
                        default=DEFAULT_LOAD_ADDRESS)
    parser.add_argument("--corpus", type=Path, required=True)
    parser.add_argument("--corpus-load", type=number,
                        default=DEFAULT_LOAD_ADDRESS)
    parser.add_argument("--minimum-size", type=number, default=24)
    parser.add_argument("--maximum-size", type=number, default=4096)
    parser.add_argument("--output", type=Path,
                        default=ROOT / "build/us/cross_corpus_matches.json")
    args = parser.parse_args()

    executable = args.target.read_bytes()
    analysis = json.loads(args.analysis.read_text())
    report = scan(
        executable[args.target_header:], analysis["functions"],
        args.corpus.read_bytes(), args.target_load, args.corpus_load,
        args.minimum_size, args.maximum_size,
    )
    report.update({
        "schema": 1,
        "target": str(args.target),
        "corpus": str(args.corpus),
        "normalization": "MIPS J/JAL destination fields only",
    })
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    print(
        f"wrote {args.output}: {len(report['exact_hits'])} exact and "
        f"{len(report['jump_normalized_hits'])} jump-normalized hits"
    )


if __name__ == "__main__":
    main()
