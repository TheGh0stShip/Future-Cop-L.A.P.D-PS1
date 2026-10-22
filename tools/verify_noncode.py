#!/usr/bin/env python3
"""Verify declarative non-code reconstructions against user-supplied inputs."""

from __future__ import annotations

import argparse
import hashlib
import json
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
LOAD_ADDRESS = 0x80010000
HEADER_SIZE = 0x800


def number(value: int | str) -> int:
    return int(value, 0) if isinstance(value, str) else value


def verify_executable(executable: bytes, source_path: Path) -> tuple[int, int]:
    source = json.loads(source_path.read_text())
    seen = []
    matched = 0
    for item in source["ranges"]:
        address, size = number(item["address"]), number(item["size"])
        value = number(item["fill"])
        if not 0 <= value <= 0xFF or size <= 0:
            raise ValueError(f"invalid fill recipe at 0x{address:08X}")
        start = HEADER_SIZE + address - LOAD_ADDRESS
        expected = executable[start:start + size]
        if len(expected) != size or expected != bytes([value]) * size:
            raise ValueError(f"fill recipe does not match executable at 0x{address:08X}")
        seen.append((address, address + size))
        matched += size
    seen.sort()
    for left, right in zip(seen, seen[1:]):
        if left[1] > right[0]:
            raise ValueError(f"overlapping executable fill recipes at 0x{right[0]:08X}")
    return len(seen), matched


def verify_assets(files_root: Path, manifest_path: Path) -> tuple[int, int]:
    manifest = json.loads(manifest_path.read_text())
    count = 0
    matched = 0
    for unit in manifest["units"]:
        path = files_root / unit["path"]
        data = path.read_bytes()
        size, value = number(unit["size"]), number(unit["fill"])
        if len(data) != size or data != bytes([value]) * size:
            raise ValueError(f"asset fill recipe does not match {unit['path']}")
        digest = hashlib.sha256(data).hexdigest()
        if digest != unit["sha256"]:
            raise ValueError(f"asset digest mismatch for {unit['path']}")
        count += 1
        matched += size
    return count, matched


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executable", type=Path,
                        default=ROOT / "baseroms/us/SLUS_007.39")
    parser.add_argument("--files", type=Path, default=ROOT / "build/disc/files")
    parser.add_argument("--executable-source", type=Path,
                        default=ROOT / "src/data/executable_padding.json")
    parser.add_argument("--asset-manifest", type=Path,
                        default=ROOT / "config/us/matching_assets.json")
    args = parser.parse_args()
    data_units, data_bytes = verify_executable(
        args.executable.read_bytes(), args.executable_source
    )
    asset_units, asset_bytes = verify_assets(args.files, args.asset_manifest)
    print(f"verified {data_units} executable data ranges / {data_bytes} bytes and "
          f"{asset_units} assets / {asset_bytes} bytes")


if __name__ == "__main__":
    main()
