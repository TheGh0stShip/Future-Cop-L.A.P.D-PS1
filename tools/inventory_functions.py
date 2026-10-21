#!/usr/bin/env python3
"""Inventory provisional function starts from Splat-generated assembly."""

import argparse
import json
import re
from pathlib import Path

TEXT_START = 0x80010000
TEXT_END = 0x8009F800
FUNCTION_RE = re.compile(r"^glabel func_([0-9A-Fa-f]{8})$")
NAMED_FUNCTIONS = {"glabel entrypoint": 0x80096520}


def scan(text: str) -> list[int]:
    addresses = []
    for line in text.splitlines():
        stripped = line.strip()
        match = FUNCTION_RE.match(stripped)
        if match:
            addresses.append(int(match.group(1), 16))
        elif stripped in NAMED_FUNCTIONS:
            addresses.append(NAMED_FUNCTIONS[stripped])
    if addresses != sorted(set(addresses)):
        raise ValueError("function candidates are duplicated or out of order")
    if any(not TEXT_START <= address < TEXT_END or address & 3 for address in addresses):
        raise ValueError("candidate outside loaded text or not word-aligned")
    return addresses


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("assembly", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    addresses = scan(args.assembly.read_text())
    report = {"schema": 1, "classification": "provisional call-target candidates",
              "loaded_range": [TEXT_START, TEXT_END], "count": len(addresses),
              "addresses": addresses,
              "warning": "Candidates are not reviewed boundaries and award no matching progress."}
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(report, indent=2) + "\n")
    print(f"inventoried {len(addresses)} provisional function starts")
