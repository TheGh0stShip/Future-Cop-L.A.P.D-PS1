#!/usr/bin/env python3
"""Inventory executable non-function ranges and disc asset files."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
LOAD_START = 0x80010000
LOAD_END = 0x8009F800


def write_json(path: Path, value: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")


def executable_ranges(analysis_path: Path) -> dict:
    analysis = json.loads(analysis_path.read_text())
    functions = sorted(
        (item["addr"], item["end"])
        for item in analysis["functions"]
        if not item.get("is_data") and not item.get("alias_of")
    )
    cursor = LOAD_START
    ranges = []
    for start, end in functions:
        if start < cursor or not LOAD_START <= start < end <= LOAD_END:
            raise ValueError(f"invalid canonical function range 0x{start:08X}..0x{end:08X}")
        if start > cursor:
            ranges.append({"address": f"0x{cursor:08X}", "size": f"0x{start - cursor:X}"})
        cursor = end
    if cursor < LOAD_END:
        ranges.append({"address": f"0x{cursor:08X}", "size": f"0x{LOAD_END - cursor:X}"})
    total = sum(int(item["size"], 0) for item in ranges)
    return {
        "schema": 1,
        "classification": "executable bytes outside canonical function ranges",
        "loaded_range": [f"0x{LOAD_START:08X}", f"0x{LOAD_END:08X}"],
        "total_bytes": total,
        "ranges": ranges,
        "warning": "Ranges may contain initialized data, padding, or uncatalogued code; classification must be reviewed before matching credit.",
    }


def category(path: Path) -> str:
    suffix = path.suffix.upper()
    return {
        ".MIS": "missions",
        ".LSC": "level_scripts",
        ".DEM": "recordings",
        ".WVE": "movies",
        ".FSV": "movies",
        ".DAT": "padding",
        ".CNF": "configuration",
    }.get(suffix, "unknown")


def asset_ledger(files_root: Path) -> dict:
    assets = []
    for path in sorted(item for item in files_root.rglob("*") if item.is_file()):
        relative = path.relative_to(files_root).as_posix()
        if relative in {".extracted", "SLUS_007.39"}:
            continue
        data = path.read_bytes()
        assets.append({
            "path": relative,
            "size": len(data),
            "sha256": hashlib.sha256(data).hexdigest(),
            "category": category(path),
        })
    return {
        "schema": 1,
        "classification": "disc filesystem assets excluding the boot executable",
        "total_bytes": sum(item["size"] for item in assets),
        "files": assets,
        "warning": "Inventory and extraction alone award no matching progress.",
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--analysis", type=Path, default=ROOT / "analysis/analysis.json")
    parser.add_argument("--files", type=Path, default=ROOT / "build/disc/files")
    parser.add_argument("--data-output", type=Path,
                        default=ROOT / "config/us/executable_data_ranges.json")
    parser.add_argument("--asset-output", type=Path,
                        default=ROOT / "config/us/asset_ledger.json")
    args = parser.parse_args()
    data = executable_ranges(args.analysis)
    assets = asset_ledger(args.files)
    write_json(args.data_output, data)
    write_json(args.asset_output, assets)
    print(f"inventoried {data['total_bytes']} executable non-function bytes and "
          f"{assets['total_bytes']} asset bytes")


if __name__ == "__main__":
    main()
