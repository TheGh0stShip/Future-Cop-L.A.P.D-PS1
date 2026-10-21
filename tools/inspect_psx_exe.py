#!/usr/bin/env python3
"""Print the fixed-width header fields of a Sony PS-X EXE."""

from __future__ import annotations

import argparse
import hashlib
import struct
from pathlib import Path


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("executable", type=Path)
    args = parser.parse_args()
    data = args.executable.read_bytes()
    if len(data) < 0x800 or data[:8] != b"PS-X EXE":
        raise ValueError("not a PS-X EXE")
    fields = struct.unpack_from("<8I", data, 0x10)
    names = ("initial_pc", "initial_gp", "load_address", "text_size",
             "data_address", "data_size", "bss_address", "bss_size")
    print(f"path={args.executable}")
    print(f"file_size=0x{len(data):x}")
    print(f"sha256={hashlib.sha256(data).hexdigest()}")
    for name, value in zip(names, fields):
        print(f"{name}=0x{value:08x}")
    sp, sp_offset = struct.unpack_from("<2I", data, 0x30)
    print(f"stack_address=0x{sp:08x}")
    print(f"stack_offset=0x{sp_offset:08x}")


if __name__ == "__main__":
    main()
