#!/usr/bin/env python3
"""Locate PsyQ objects and named functions in a PS-X EXE payload."""

from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path


HEADER_SIZE = 0x800
PLACEHOLDER = re.compile(
    r"^(?:text|data|rdata|bss|sbss|sdata|loc|locret|sub|unk|byte|word|dword"
    r"|qword|off|asc|stru|flt|dbl|jpt|def)_[0-9A-Fa-f]+$"
)


def parse_signature(text: str) -> tuple[bytes, bytes]:
    values = bytearray()
    masks = bytearray()
    for token in text.split():
        if token == "??":
            values.append(0)
            masks.append(0)
        elif re.fullmatch(r"[0-9A-Fa-f]{2}", token):
            values.append(int(token, 16))
            masks.append(0xFF)
        else:
            raise ValueError(f"invalid signature token: {token!r}")
    if not values:
        raise ValueError("empty signature")
    return bytes(values), bytes(masks)


def longest_anchor(values: bytes, masks: bytes) -> tuple[int, bytes]:
    best_start = 0
    best = b""
    start = 0
    while start < len(masks):
        while start < len(masks) and not masks[start]:
            start += 1
        end = start
        while end < len(masks) and masks[end]:
            end += 1
        if end - start > len(best):
            best_start = start
            best = values[start:end]
        start = end + 1
    return best_start, best


def masked_matches(payload: bytes, values: bytes, masks: bytes) -> list[int]:
    anchor_offset, anchor = longest_anchor(values, masks)
    if len(anchor) < 4:
        return []
    matches: list[int] = []
    position = payload.find(anchor)
    while position >= 0:
        candidate = position - anchor_offset
        if candidate >= 0 and candidate + len(values) <= len(payload):
            view = payload[candidate : candidate + len(values)]
            if all(not mask or actual == expected for actual, expected, mask in zip(view, values, masks)):
                matches.append(candidate)
        position = payload.find(anchor, position + 1)
    return matches


def catalogue_digest(directory: Path) -> str:
    digest = hashlib.sha256(b"psyq-signature-catalogue-v1\0")
    for path in sorted(directory.glob("*.json")):
        name = path.name.encode()
        data = path.read_bytes()
        digest.update(len(name).to_bytes(4, "big"))
        digest.update(name)
        digest.update(len(data).to_bytes(8, "big"))
        digest.update(data)
    return digest.hexdigest()


def load_function_starts(path: Path) -> set[int]:
    document = json.loads(path.read_text())
    return {int(value) for value in document["addresses"]}


def scan(executable: Path, inventory: Path, catalogues: list[Path]) -> dict[str, object]:
    image = executable.read_bytes()
    if image[:8] != b"PS-X EXE":
        raise ValueError(f"{executable}: not a PS-X EXE")
    load_address = struct.unpack_from("<I", image, 0x18)[0]
    payload_size = struct.unpack_from("<I", image, 0x1C)[0]
    payload = image[HEADER_SIZE : HEADER_SIZE + payload_size]
    starts = load_function_starts(inventory)
    objects: list[dict[str, object]] = []
    proposals: dict[int, list[dict[str, object]]] = {}

    for catalogue in catalogues:
        for path in sorted(catalogue.glob("*.json")):
            entries = json.loads(path.read_text())
            for entry in entries:
                if not entry.get("sig"):
                    continue
                values, masks = parse_signature(entry["sig"])
                hits = masked_matches(payload, values, masks)
                if len(hits) != 1:
                    continue
                base = load_address + hits[0]
                record = {
                    "version": catalogue.name,
                    "library": path.name.removesuffix(".json"),
                    "object": entry["name"],
                    "address": f"0x{base:08X}",
                    "size": len(values),
                }
                objects.append(record)
                for label in entry.get("labels", []):
                    name = label["name"]
                    address = base + int(label["offset"])
                    if PLACEHOLDER.match(name) or address not in starts:
                        continue
                    proposals.setdefault(address, []).append({**record, "name": name})

    proposal_rows = []
    for address, rows in sorted(proposals.items()):
        names = sorted({row["name"] for row in rows})
        proposal_rows.append(
            {
                "address": f"0x{address:08X}",
                "names": names,
                "unambiguous": len(names) == 1,
                "providers": rows,
            }
        )
    return {
        "schema": 1,
        "executable": str(executable),
        "load_address": f"0x{load_address:08X}",
        "payload_size": payload_size,
        "catalogues": [
            {"path": str(path), "sha256": catalogue_digest(path)} for path in catalogues
        ],
        "unique_objects": objects,
        "proposals": proposal_rows,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--executable", type=Path, required=True)
    parser.add_argument("--inventory", type=Path, required=True)
    parser.add_argument("--signatures", type=Path, action="append", required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = scan(args.executable, args.inventory, args.signatures)
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(result, indent=2) + "\n")
    print(
        f"wrote {args.output}: {len(result['unique_objects'])} unique objects, "
        f"{len(result['proposals'])} function proposals"
    )


if __name__ == "__main__":
    main()
