#!/usr/bin/env python3
"""Generate the objdiff v2 progress report consumed by public trackers."""

from __future__ import annotations

import argparse
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
ANALYSIS = ROOT / "analysis/analysis.json"
MATCHING_UNITS = ROOT / "config/us/matching_units.json"
BOUNDARY_OVERRIDES = ROOT / "config/us/function_boundary_overrides.json"
DEFAULT_OUTPUT = ROOT / "build/report.json"


def measures(total_code: int, matched_code: int, total_functions: int,
             matched_functions: int) -> dict:
    code_percent = matched_code * 100.0 / total_code if total_code else 0.0
    function_percent = (
        matched_functions * 100.0 / total_functions if total_functions else 0.0
    )
    return {
        "fuzzy_match_percent": code_percent,
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "matched_code_percent": code_percent,
        "total_data": "0",
        "matched_data": "0",
        "matched_data_percent": 0.0,
        "total_functions": total_functions,
        "matched_functions": matched_functions,
        "matched_functions_percent": function_percent,
        "complete_code": str(matched_code),
        "complete_code_percent": code_percent,
        "complete_data": "0",
        "complete_data_percent": 0.0,
        "total_units": total_functions,
        "complete_units": matched_functions,
    }


def canonical_functions() -> list[dict]:
    analysis = json.loads(ANALYSIS.read_text())
    functions = [
        function for function in analysis["functions"]
        if not function.get("is_data") and not function.get("alias_of")
    ]
    if BOUNDARY_OVERRIDES.is_file():
        overrides = json.loads(BOUNDARY_OVERRIDES.read_text())
        by_address = {function["addr"]: function for function in functions}
        for override in overrides["functions"]:
            address = int(override["address"], 0)
            function = by_address.get(address)
            if function is None:
                raise ValueError(
                    f"boundary override start is not catalogued: 0x{address:08X}"
                )
            for value in override.get("remove_starts", []):
                removed = by_address.pop(int(value, 0), None)
                if removed is None:
                    raise ValueError(f"boundary override removal is not catalogued: {value}")
            function["size"] = int(override["size"], 0)
            function["end"] = address + function["size"]
        functions = list(by_address.values())
    functions.sort(key=lambda function: function["addr"])
    for left, right in zip(functions, functions[1:]):
        if left["end"] > right["addr"]:
            raise ValueError(
                f"overlapping function ranges at 0x{right['addr']:08X}"
            )
    return functions


def matching_units() -> dict[int, dict]:
    manifest = json.loads(MATCHING_UNITS.read_text())
    units = {}
    for unit in manifest["units"]:
        address = int(unit["address"], 0)
        size = int(unit["size"], 0)
        if address in units:
            raise ValueError(f"duplicate matching unit at 0x{address:08X}")
        units[address] = {**unit, "size_int": size}
    return units


def build_report() -> dict:
    functions = canonical_functions()
    matched = matching_units()
    catalog = {function["addr"]: function for function in functions}

    for address, unit in matched.items():
        function = catalog.get(address)
        if function is None:
            raise ValueError(f"matching unit is not catalogued: 0x{address:08X}")
        if function["size"] != unit["size_int"]:
            raise ValueError(
                f"size mismatch at 0x{address:08X}: catalogue "
                f"0x{function['size']:X}, manifest 0x{unit['size_int']:X}"
            )

    units = []
    matched_bytes = 0
    for function in functions:
        address = function["addr"]
        size = function["size"]
        match = matched.get(address)
        complete = match is not None
        byte_count = size if complete else 0
        matched_bytes += byte_count
        unit_measures = measures(size, byte_count, 1, int(complete))
        units.append({
            "name": function["name"],
            "measures": unit_measures,
            "functions": [{
                "name": function["name"],
                "size": str(size),
                "fuzzy_match_percent": unit_measures["fuzzy_match_percent"],
                "address": str(address),
                "metadata": {"virtual_address": str(address)},
            }],
            "metadata": {
                "complete": complete,
                "source_path": (
                    match["source"] if complete
                    else f"asm/us/functions/{function['name']}.s"
                ),
                "progress_categories": ["game"],
                "auto_generated": not complete,
            },
        })

    total_bytes = sum(function["size"] for function in functions)
    aggregate = measures(total_bytes, matched_bytes, len(functions), len(matched))
    return {
        "measures": aggregate,
        "units": units,
        "version": 2,
        "categories": [{
            "id": "game",
            "name": "Game code",
            "measures": aggregate,
        }],
    }


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--output", type=Path, default=DEFAULT_OUTPUT)
    args = parser.parse_args()
    report = build_report()
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    result = report["measures"]
    print(
        f"wrote {args.output}: {result['matched_code']} / "
        f"{result['total_code']} code bytes, "
        f"{result['matched_functions']} / {result['total_functions']} functions"
    )


if __name__ == "__main__":
    main()
