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
EXECUTABLE_DATA = ROOT / "config/us/executable_data_ranges.json"
EXECUTABLE_PADDING = ROOT / "src/data/executable_padding.json"
ASSET_LEDGER = ROOT / "config/us/asset_ledger.json"
MATCHING_ASSETS = ROOT / "config/us/matching_assets.json"
DEFAULT_OUTPUT = ROOT / "build/report.json"


def measures(total_code: int, matched_code: int, total_functions: int,
             matched_functions: int, total_data: int = 0,
             matched_data: int = 0) -> dict:
    code_percent = matched_code * 100.0 / total_code if total_code else 0.0
    data_percent = matched_data * 100.0 / total_data if total_data else 0.0
    function_percent = (
        matched_functions * 100.0 / total_functions if total_functions else 0.0
    )
    return {
        "fuzzy_match_percent": code_percent,
        "total_code": str(total_code),
        "matched_code": str(matched_code),
        "matched_code_percent": code_percent,
        "total_data": str(total_data),
        "matched_data": str(matched_data),
        "matched_data_percent": data_percent,
        "total_functions": total_functions,
        "matched_functions": matched_functions,
        "matched_functions_percent": function_percent,
        "complete_code": str(matched_code),
        "complete_code_percent": code_percent,
        "complete_data": str(matched_data),
        "complete_data_percent": data_percent,
        "total_units": total_functions,
        "complete_units": matched_functions,
    }


def data_measures(total: int, matched: int) -> dict:
    """Expose data progress in category panels that display code measures."""
    result = measures(total, matched, 0, 0, total_data=total, matched_data=matched)
    result["fuzzy_match_percent"] = result["matched_data_percent"]
    result["total_code"] = str(total)
    result["matched_code"] = str(matched)
    result["matched_code_percent"] = result["matched_data_percent"]
    result["complete_code"] = str(matched)
    result["complete_code_percent"] = result["matched_data_percent"]
    return result


def noncode_progress() -> tuple[int, int, int, int, dict[str, tuple[int, int]]]:
    executable = json.loads(EXECUTABLE_DATA.read_text())
    padding = json.loads(EXECUTABLE_PADDING.read_text())
    ledger = json.loads(ASSET_LEDGER.read_text())
    matching = json.loads(MATCHING_ASSETS.read_text())

    executable_total = executable["total_bytes"]
    executable_matched = sum(int(item["size"], 0) for item in padding["ranges"])
    ledger_by_path = {item["path"]: item for item in ledger["files"]}
    matched_by_category: dict[str, int] = {}
    for unit in matching["units"]:
        item = ledger_by_path.get(unit["path"])
        if item is None:
            raise ValueError(f"matching asset is absent from ledger: {unit['path']}")
        if item["size"] != int(unit["size"]):
            raise ValueError(f"matching asset size differs from ledger: {unit['path']}")
        category = item["category"]
        matched_by_category[category] = matched_by_category.get(category, 0) + item["size"]

    category_totals: dict[str, int] = {}
    for item in ledger["files"]:
        category = item["category"]
        category_totals[category] = category_totals.get(category, 0) + item["size"]
    categories = {
        category: (total, matched_by_category.get(category, 0))
        for category, total in category_totals.items()
    }
    return (executable_total, executable_matched, ledger["total_bytes"],
            sum(matched_by_category.values()), categories)


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
    executable_total, executable_matched, asset_total, asset_matched, asset_categories = (
        noncode_progress()
    )
    aggregate = measures(
        total_bytes, matched_bytes, len(functions), len(matched),
        total_data=executable_total + asset_total,
        matched_data=executable_matched + asset_matched,
    )
    categories = [{
        "id": "game",
        "name": "Game code",
        "measures": measures(total_bytes, matched_bytes, len(functions), len(matched)),
    }, {
        "id": "executable_data",
        "name": "Executable data",
        "measures": data_measures(executable_total, executable_matched),
    }, {
        "id": "disc_assets",
        "name": "Disc assets",
        "measures": data_measures(asset_total, asset_matched),
    }]
    labels = {
        "missions": "Missions",
        "level_scripts": "Level scripts",
        "recordings": "Recordings",
        "movies": "Movies",
        "padding": "Disc padding",
        "configuration": "Configuration",
        "unknown": "Unclassified assets",
    }
    for category, (total, matched_bytes_in_category) in sorted(asset_categories.items()):
        categories.append({
            "id": f"assets_{category}",
            "name": labels.get(category, category.replace("_", " ").title()),
            "measures": data_measures(total, matched_bytes_in_category),
        })
    return {
        "measures": aggregate,
        "units": units,
        "version": 2,
        "categories": categories,
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
