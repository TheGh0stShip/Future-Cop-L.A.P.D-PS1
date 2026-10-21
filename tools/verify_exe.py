#!/usr/bin/env python3
"""Verify the supported USA executable and its PS-X EXE memory contract."""

import argparse
import hashlib
import struct
from pathlib import Path

EXPECTED_SIZE = 0x90000
EXPECTED_SHA1 = "979ccf0ae0027a569a35a08f38e5d58a00d8db9b"
EXPECTED_SHA256 = "892f8482d3c9b163f45afae50676808e5673bdb7d6c079663cc1fdf6826b236f"
EXPECTED_FIELDS = {0x10: ("entry point", 0x80096520), 0x14: ("initial gp", 0),
                   0x18: ("load address", 0x80010000), 0x1C: ("text size", 0x8F800),
                   0x30: ("initial sp", 0x801FFFF0)}


def verify(path: Path) -> None:
    data = path.read_bytes()
    errors = []
    if len(data) != EXPECTED_SIZE:
        errors.append(f"size 0x{len(data):X} != 0x{EXPECTED_SIZE:X}")
    if data[:8] != b"PS-X EXE":
        errors.append("missing PS-X EXE magic")
    if hashlib.sha1(data).hexdigest() != EXPECTED_SHA1:
        errors.append("SHA-1 does not match SLUS-00739")
    if hashlib.sha256(data).hexdigest() != EXPECTED_SHA256:
        errors.append("SHA-256 does not match SLUS-00739")
    for offset, (name, expected) in EXPECTED_FIELDS.items():
        actual = struct.unpack_from("<I", data, offset)[0]
        if actual != expected:
            errors.append(f"{name} 0x{actual:08X} != 0x{expected:08X}")
    if errors:
        raise SystemExit("verification failed:\n  " + "\n  ".join(errors))
    print(f"verified {path}")
    print("header: 0x800 bytes")
    print("loaded image: 0x8F800 bytes at 0x80010000")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("executable", type=Path)
    verify(parser.parse_args().executable)
