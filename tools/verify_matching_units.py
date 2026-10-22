#!/usr/bin/env python3
"""Compile reviewed C units and require exact bytes against SLUS-00739."""

import argparse
import hashlib
import json
import os
import re
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


def tool_environment(environment=None):
    result = dict(os.environ if environment is None else environment)
    library_dir = (
        ROOT / ".toolchain/mipsel-binutils/root/usr/lib/x86_64-linux-gnu"
    )
    if library_dir.is_dir():
        current = result.get("LD_LIBRARY_PATH", "")
        result["LD_LIBRARY_PATH"] = str(library_dir) + (
            os.pathsep + current if current else ""
        )
    return result


def reorder_aspsx_return_delay_slot(text):
    """Reproduce ASPSX's safe stack-pop fill for a terminal return."""
    lines = text.splitlines()
    for index in range(len(lines) - 2):
        stack_pop = re.fullmatch(r"\s*addu\s+\$sp,\$sp,(-?(?:0x)?[0-9a-fA-F]+)\s*", lines[index])
        return_jump = re.fullmatch(r"\s*j\s+\$31\s*", lines[index + 1])
        delay_nop = re.fullmatch(r"\s*nop(?:\s+#.*)?\s*", lines[index + 2])
        if stack_pop and return_jump and delay_nop:
            lines[index:index + 3] = [lines[index + 1], lines[index]]
            return "\n".join(lines) + "\n"
    raise ValueError("ASPSX return-delay-slot pattern not found")


def reorder_aspsx_jump_store_delay_slot(text):
    """Reproduce ASPSX's symbolic-store fill for a direct jump or call."""
    lines = text.splitlines()
    for index in range(len(lines) - 2):
        store = re.fullmatch(r"\s*sw\s+(\$[0-9]+),([^\s]+)\s*", lines[index])
        jump = re.fullmatch(r"\s*(?:j|jal)\s+(?:\$31|\$L[0-9A-Za-z_]+|[A-Za-z_][A-Za-z0-9_]*)\s*", lines[index + 1])
        delay_nop = re.fullmatch(r"\s*nop(?:\s+#.*)?\s*", lines[index + 2])
        if store and jump and delay_nop:
            register, destination = store.groups()
            if "(" not in destination:
                lines[index:index + 3] = [
                    ".set\tnoat",
                    f"lui\t$at,%hi({destination})",
                    lines[index + 1],
                    f"sw\t{register},%lo({destination})($at)",
                    ".set\tat",
                ]
            else:
                lines[index:index + 3] = [lines[index + 1], lines[index]]
            return "\n".join(lines) + "\n"
    raise ValueError("ASPSX jump-store-delay-slot pattern not found")


def move_aspsx_load_delay_label(text):
    """Place a loop label after an ASPSX-inserted load-delay nop."""
    lines = text.splitlines()
    for index in range(len(lines) - 2):
        label = re.fullmatch(r"\s*(\$L[0-9A-Za-z_]+):\s*", lines[index])
        delay_nop = re.fullmatch(r"\s*nop\s+# DEBUG: Reuse of .*", lines[index + 1])
        skipped = re.fullmatch(
            r"\s*#\s*(\$L[0-9A-Za-z_]+):\s+# DEBUG: skipped\s*",
            lines[index + 2],
        )
        if label and delay_nop and skipped and label.group(1) == skipped.group(1):
            lines[index:index + 3] = [lines[index + 1], lines[index]]
            return "\n".join(lines) + "\n"
    raise ValueError("ASPSX load-delay-label pattern not found")


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
        if not isinstance(unit.get("link", False), bool):
            raise ValueError(f"{unit['name']}: link must be boolean")
        if not isinstance(unit.get("aspsx_reorder_epilogue", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reorder_epilogue must be boolean"
            )
        if not isinstance(unit.get("aspsx_move_load_delay_label", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_move_load_delay_label must be boolean"
            )
        if not isinstance(unit.get("aspsx_reorder_jump_store", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reorder_jump_store must be boolean"
            )
        if unit.get("link", False):
            linker_symbols = unit.get("linker_symbols", {})
            if not isinstance(linker_symbols, dict):
                raise ValueError(f"{unit['name']}: linker_symbols must be an object")
            for symbol, value in linker_symbols.items():
                if not isinstance(symbol, str) or not symbol:
                    raise ValueError(f"{unit['name']}: invalid linker symbol")
                number(value)
        ranges.append((start, start + size, unit["name"]))
    ranges.sort()
    for left, right in zip(ranges, ranges[1:]):
        if left[1] > right[0]:
            raise ValueError(f"overlapping units: {left[2]} and {right[2]}")


def compile_unit(unit, config, prefix, work, environment=None):
    environment = tool_environment(environment)
    compiler = unit.get("compiler", config["compiler"])
    cc1 = ROOT / ".toolchain" / compiler / "cc1"
    if not cc1.is_file():
        raise SystemExit(f"missing {cc1}; run make toolchain")
    source = ROOT / unit["source"]
    preprocessed = work / "unit.i"
    raw_asm = work / "raw.s"
    maspsx_input = work / "maspsx-input.s"
    processed = work / "processed.s"
    linked_asm = work / "linked.s"
    obj = work / "unit.o"
    linked = work / "unit.elf"
    linker_script = work / "unit.ld"
    binary = work / "unit.bin"
    flags = unit.get("compiler_flags", config["compiler_flags"])
    cpp = ROOT / ".toolchain" / compiler / "cpp"
    if not cpp.is_file():
        raise SystemExit(f"missing {cpp}; run make toolchain")
    run([cpp, "-lang-c", "-I", ROOT / "include", source, preprocessed], env=environment)
    run([cc1, *flags, preprocessed, "-o", raw_asm], env=environment)

    maspsx = ROOT / ".toolchain" / f"maspsx-{MASPSX_COMMIT}" / "maspsx.py"
    maspsx_flags = list(unit.get("maspsx_flags", config["maspsx_flags"]))
    maspsx_flags.extend(
        flag for flag in flags if flag.startswith("-G") and len(flag) > 2
    )
    small_data_symbols = unit.get("small_data_symbols", {})
    marker_lines = [
        f".comm {name},{number(size)}"
        for name, size in sorted(small_data_symbols.items())
    ]
    maspsx_input.write_text(
        "\n".join(marker_lines) + ("\n" if marker_lines else "")
        + raw_asm.read_text()
    )
    with processed.open("w") as output:
        run(["python3", maspsx, *maspsx_flags, maspsx_input],
            stdout=output, env=environment)
    if unit.get("aspsx_reorder_epilogue", False):
        processed.write_text(reorder_aspsx_return_delay_slot(processed.read_text()))
    if unit.get("aspsx_move_load_delay_label", False):
        processed.write_text(move_aspsx_load_delay_label(processed.read_text()))
    if unit.get("aspsx_reorder_jump_store", False):
        processed.write_text(
            reorder_aspsx_jump_store_delay_slot(processed.read_text())
        )
    if marker_lines:
        marker_names = set(small_data_symbols)
        processed.write_text("\n".join(
            line for line in processed.read_text().splitlines()
            if not (
                line.lstrip().startswith(".comm")
                and line.lstrip().split(None, 1)[1].split(",", 1)[0]
                in marker_names
            )
        ) + "\n")
    link_defined = set(unit.get("linker_symbols", {}))
    link_defined.update(small_data_symbols)
    definitions = "".join(
        f".set {name}, {number(value):#x}\n"
        for name, value in sorted(unit["symbols"].items())
        if not unit.get("link", False) or name not in link_defined
    )
    linked_asm.write_text(definitions + processed.read_text())

    run([tool("as", prefix), "-EL", "-march=r3000", "-mabi=32", "-o", obj, linked_asm],
        env=environment)
    binary_input = obj
    if unit.get("link", False):
        address = number(unit["address"])
        linker_script.write_text(
            "SECTIONS\n{\n"
            f"  .text {address:#x} : SUBALIGN(4) {{ *(.text) }}\n"
            "  /DISCARD/ : { *(.MIPS.abiflags) *(.reginfo) *(.pdr) "
            "*(.gnu.attributes) }\n"
            "}\n"
        )
        command = [tool("ld", prefix), "-EL", "-m", "elf32ltsmip", "-T",
                   linker_script, "-e", f"{address:#x}", "-o", linked, obj]
        linker_symbols = dict(unit.get("linker_symbols", {}))
        linker_symbols.update({
            name: unit["symbols"][name] for name in small_data_symbols
        })
        for name, value in sorted(linker_symbols.items()):
            command.extend(["--defsym", f"{name}={number(value):#x}"])
        run(command, env=environment)
        binary_input = linked
    run([tool("objcopy", prefix), "-O", "binary", "-j", ".text", binary_input, binary],
        env=environment)
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
