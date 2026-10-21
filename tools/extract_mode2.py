#!/usr/bin/env python3
"""Convert a raw MODE2/2352 XA track to a 2048-byte-sector ISO view."""

from __future__ import annotations

import argparse
from pathlib import Path

RAW_SECTOR = 2352
USER_OFFSET = 24
USER_SIZE = 2048
SYNC = bytes.fromhex("00ffffffffffffffffffff00")


def convert(source: Path, destination: Path) -> tuple[int, int]:
    size = source.stat().st_size
    if size % RAW_SECTOR:
        raise ValueError(f"input size {size} is not a multiple of {RAW_SECTOR}")

    sectors = size // RAW_SECTOR
    destination.parent.mkdir(parents=True, exist_ok=True)
    form2_count = 0
    with source.open("rb") as src, destination.open("wb") as dst:
        for index in range(sectors):
            sector = src.read(RAW_SECTOR)
            if sector[:12] != SYNC:
                raise ValueError(f"sector {index}: invalid CD sync pattern")
            if sector[16:20] != sector[20:24]:
                raise ValueError(f"sector {index}: XA subheader copies differ")
            submode = sector[18]
            form2_count += bool(submode & 0x20)
            # ISO 9660 addresses 2048-byte logical blocks. XA Form 2 sectors
            # have 276 extra user bytes; those require raw-sector handling for
            # faithful media extraction but their first logical block still
            # provides a mountable filesystem view.
            dst.write(sector[USER_OFFSET : USER_OFFSET + USER_SIZE])
    return sectors, form2_count


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    args = parser.parse_args()
    count, form2_count = convert(args.source, args.destination)
    print(f"extracted {count} MODE2 sectors ({count * USER_SIZE} bytes); "
          f"Form 1: {count - form2_count}, Form 2: {form2_count}")


if __name__ == "__main__":
    main()
