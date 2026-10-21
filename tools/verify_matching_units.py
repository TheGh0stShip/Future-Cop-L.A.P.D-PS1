#!/usr/bin/env python3
"""Compile reviewed C units and require exact bytes against SLUS-00739."""

import argparse
import hashlib
import json
import os
import shutil
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
LOAD_ADDRESS = 0x80010000
HEADER_SIZE = 0x800
MASPSX_COMMIT = "423ffc6bca05e53f9b7ab06159d0cbe71111bc90"


def number(value):
    return int(value, 0) if isinstance(value, str) else value


def tool(name, prefix):
    found = shutil.which(prefix + name)
    if found:
        return found
    local = ROOT / ".toolchain/mipsel-binutils/root/usr/bin" / (prefix + name)
    if local.is_file():
        return str(local)
    raise SystemExit(f"missing {prefix + name}; run make toolchain")


def run(args, **kwargs):
    subprocess.run([str(arg) for arg in args], check=True, **kwargs)


def validate(config):
    ranges = []
    names = set()
    for unit in config["units"]:
        required = {"name", "address", "size", "source", "symbols"}
        missing = required - unit.keys()
        if missing:
            raise ValueError(f"{unit.get('name', '<unit>')}: missing {sorted(missing)}")
        if unit["name"] in names:
            raise ValueError(f"duplicate matching unit {unit['name']}")
        names.add(unit["name"])
        start, size = number(unit["address"]), number(unit["size"])
        if start & 3 or size <= 0 or size & 3:
            raise ValueError(f"{unit['name']}: range must be positive and word-aligned")
        if not (ROOT / unit["source"]).is_file():
            raise ValueError(f"{unit['name']}: source does not exist")
        ranges.append((start, start + size, unit["name"]))
    ranges.sort()
    for left, right in zip(ranges, ranges[1:]):
        if left[1] > right[0]:
            raise ValueError(f"overlapping units: {left[2]} and {right[2]}")


def compile_unit(unit, config, prefix, work):
    compiler = unit.get("compiler", config["compiler"])
    cc1 = ROOT / ".toolchain" / compiler / "cc1"
    if not cc1.is_file():
        raise SystemExit(f"missing {cc1}; run make toolchain")
    source = ROOT / unit["source"]
    preprocessed = work / "unit.i"
    raw_asm = work / "raw.s"
    processed = work / "processed.s"
    linked_asm = work / "linked.s"
    obj = work / "unit.o"
    binary = work / "unit.bin"
    flags = unit.get("compiler_flags", config["compiler_flags"])
    cpp = ROOT / ".toolchain" / compiler / "cpp"
    if not cpp.is_file():
        raise SystemExit(f"missing {cpp}; run make toolchain")
    run([cpp, "-lang-c", source, preprocessed])
    run([cc1, *flags, preprocessed, "-o", raw_asm])

    maspsx = ROOT / ".toolchain" / f"maspsx-{MASPSX_COMMIT}" / "maspsx.py"
    with processed.open("w") as output:
        run(["python3", maspsx, *unit.get("maspsx_flags", config["maspsx_flags"]),
             raw_asm], stdout=output)
    definitions = "".join(f".set {name}, {number(value):#x}\n"
                          for name, value in sorted(unit["symbols"].items()))
    linked_asm.write_text(definitions + processed.read_text())

    run([tool("as", prefix), "-EL", "-march=r3000", "-mabi=32", "-o", obj, linked_asm])
    run([tool("objcopy", prefix), "-O", "binary", "-j", ".text", obj, binary])
    return binary.read_bytes()


def verify(config_path, executable_path, output_path, prefix, keep_going=False):
    config = json.loads(config_path.read_text())
    validate(config)
    executable = executable_path.read_bytes()
    results = []
    failures = []
    with tempfile.TemporaryDirectory(prefix="future-cop-match-") as directory:
        for index, unit in enumerate(config["units"]):
            work = Path(directory) / str(index)
            work.mkdir()
            actual = compile_unit(unit, config, prefix, work)
            address, size = number(unit["address"]), number(unit["size"])
            offset = HEADER_SIZE + address - LOAD_ADDRESS
            expected = executable[offset:offset + size]
            if len(expected) != size:
                raise ValueError(f"{unit['name']}: range outside executable")
            if len(actual) < size:
                actual += bytes(size - len(actual))
            failure = None
            if any(actual[size:]):
                failure = f"emitted nonzero bytes after its extent"
            elif actual[:size] != expected:
                mismatch = next(i for i in range(size) if actual[i] != expected[i])
                failure = f"differs at +0x{mismatch:X}"
            if failure is not None:
                if not keep_going:
                    raise SystemExit(f"{unit['name']} {failure}")
                failures.append({"name": unit["name"], "reason": failure})
                print(f"FAIL {unit['name']}: {failure}")
                continue
            results.append({"name": unit["name"], "address": address, "size": size,
                            "source_sha256": hashlib.sha256(
                                (ROOT / unit["source"]).read_bytes()).hexdigest(),
                            "matched": True})
            print(f"PASS {unit['name']}: {size} bytes")
    report = {"schema": 1, "compiler": config["compiler"],
              "matched_bytes": sum(row["size"] for row in results),
              "matched_functions": len(results), "units": results,
              "failures": failures}
    output_path.parent.mkdir(parents=True, exist_ok=True)
    output_path.write_text(json.dumps(report, indent=2, sort_keys=True) + "\n")
    return report


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", type=Path, default=Path("config/us/matching_units.json"))
    parser.add_argument("--executable", type=Path, default=Path("baseroms/us/SLUS_007.39"))
    parser.add_argument("--output", type=Path, default=Path("build/us/matching_report.json"))
    parser.add_argument("--binutils-prefix", default=os.environ.get("MIPSEL_PREFIX", "mipsel-linux-gnu-"))
    parser.add_argument("--keep-going", action="store_true")
    args = parser.parse_args()
    report = verify(args.config, args.executable, args.output, args.binutils_prefix,
                    keep_going=args.keep_going)
    if report["failures"]:
        raise SystemExit(1)
