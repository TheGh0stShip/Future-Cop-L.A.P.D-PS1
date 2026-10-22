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


def hoist_aspsx_small_data_load_before_prologue(text):
    """Hoist an independent small-data load ahead of saved-register setup."""
    lines = text.splitlines()
    stack_setup = re.compile(r"\s*subu\s+\$sp,\$sp,\d+\s*")
    small_load = re.compile(
        r"\s*l(?:bu|b|hu|h|w)\s+\$2,%gp_rel\([^)]+\)\(\$gp\)\s*"
    )
    setup_instruction = re.compile(
        r"\s*(?:sw|lw|addu)\s+\$\d+,[^#]+\s*"
    )
    for setup_index, line in enumerate(lines):
        if not stack_setup.fullmatch(line):
            continue
        for load_index in range(setup_index + 1, min(setup_index + 13, len(lines))):
            if not small_load.fullmatch(lines[load_index]):
                continue
            intervening = lines[setup_index + 1:load_index]
            if not all(
                not item.strip()
                or item.lstrip().startswith("#")
                or setup_instruction.fullmatch(item)
                for item in intervening
            ):
                continue
            load = lines.pop(load_index)
            lines.insert(setup_index, load)
            return "\n".join(lines) + "\n"
    raise ValueError("ASPSX small-data-load prologue pattern not found")


def reorder_aspsx_argument_save_groups(text):
    """Order adjacent saved-register argument moves by argument register."""
    lines = text.splitlines()
    store = re.compile(r"\s*sw\s+\$(\d+),(-?\d+)\(\$sp\)\s*")
    move = re.compile(r"\s*addu\s+\$(\d+),\$(\d+),\$zero\s*")
    for index in range(len(lines) - 3):
        first_store = store.fullmatch(lines[index])
        first_move = move.fullmatch(lines[index + 1])
        second_store = store.fullmatch(lines[index + 2])
        second_move = move.fullmatch(lines[index + 3])
        if not all((first_store, first_move, second_store, second_move)):
            continue
        if first_store.group(1) != first_move.group(1):
            continue
        if second_store.group(1) != second_move.group(1):
            continue
        first_argument = int(first_move.group(2))
        second_argument = int(second_move.group(2))
        if not (4 <= second_argument < first_argument <= 7):
            continue
        lines[index:index + 4] = [
            lines[index + 2], lines[index + 3], lines[index], lines[index + 1]
        ]
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX argument-save-group pattern not found")


def reuse_aspsx_symbolic_load_destination(text):
    """Use the load destination for a symbolic address when the scratch dies."""
    lines = text.splitlines()
    high = re.compile(
        r"\s*lui\s+\$(\d+),%hi\(([^)]+)\)(?:\s+#.*)?\s*"
    )
    load = re.compile(
        r"\s*(l[whbu]+)\s+\$(\d+),%lo\(([^)]+)\)\(\$(\d+)\)"
        r"(?:\s+#.*)?\s*"
    )
    symbolic_load = re.compile(
        r"\s*(l[whbu]+)\s+\$(\d+),([A-Za-z_][A-Za-z0-9_]*)\s*"
    )
    address = re.compile(
        r"\s*addiu\s+\$(\d+),\$(\d+),%lo\(([^)]+)\)(?:\s+#.*)?\s*"
    )
    overwrite = re.compile(
        r"\s*(?:li|lui|addu|addiu|ori|l[whbu]+)\s+\$(\d+)(?:,|\s).*"
    )
    for index in range(len(lines) - 2):
        symbolic_match = symbolic_load.fullmatch(lines[index])
        if symbolic_match:
            opcode, destination, symbol = symbolic_match.groups()
            lines[index:index + 1] = [
                f"lui\t${destination},%hi({symbol})",
                f"{opcode}\t${destination},%lo({symbol})(${destination})",
            ]
            return "\n".join(lines) + "\n"
        high_match = high.fullmatch(lines[index])
        load_match = load.fullmatch(lines[index + 1])
        address_match = address.fullmatch(lines[index + 1])
        if high_match and address_match:
            scratch, symbol = high_match.groups()
            destination, base, address_symbol = address_match.groups()
            if symbol == address_symbol and scratch == base and scratch != destination:
                lines[index] = f"lui\t${destination},%hi({symbol})"
                lines[index + 1] = (
                    f"addiu\t${destination},${destination},%lo({symbol})"
                )
                return "\n".join(lines) + "\n"
        next_index = index + 2
        while next_index < len(lines) and (
            not lines[next_index].strip()
            or lines[next_index].lstrip().startswith("#")
            or re.fullmatch(r"\s*nop\s+# DEBUG:.*", lines[next_index])
        ):
            next_index += 1
        overwrite_match = (
            overwrite.fullmatch(lines[next_index]) if next_index < len(lines) else None
        )
        if not all((high_match, load_match, overwrite_match)):
            continue
        scratch, symbol = high_match.groups()
        opcode, destination, load_symbol, base = load_match.groups()
        if symbol != load_symbol or scratch != base or scratch == destination:
            continue
        if overwrite_match.group(1) != scratch:
            continue
        lines[index] = f"lui\t${destination},%hi({symbol})"
        lines[index + 1] = (
            f"{opcode}\t${destination},%lo({symbol})(${destination})"
        )
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX symbolic-load-destination pattern not found")


def fill_aspsx_symbolic_load_delay_slot(text):
    """Move an independent increment into a symbolic load's delay slot."""
    lines = text.splitlines()
    increment = re.compile(r"\s*addu\s+\$(\d+),\$(\d+),1\s*")
    load = re.compile(r"\s*lw\s+\$(\d+),(.+)")
    for index in range(len(lines) - 2):
        increment_match = increment.fullmatch(lines[index])
        load_match = load.fullmatch(lines[index + 1])
        if increment_match is None or load_match is None:
            continue
        if increment_match.group(1) != increment_match.group(2):
            continue
        if increment_match.group(1) == load_match.group(1):
            continue
        if f"${increment_match.group(1)}" in load_match.group(2):
            continue
        if not lines[index + 2].strip().startswith("nop"):
            continue
        lines[index:index + 3] = [lines[index + 1], lines[index]]
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX symbolic load-delay increment pattern not found")


def reorder_aspsx_vector_copy_call_setup(text):
    """Reproduce retail scheduling around a copied vector and following call."""
    lines = text.splitlines()
    meaningful = [
        index for index, line in enumerate(lines)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    for offset in range(len(meaningful) - 10):
        indices = meaningful[offset:offset + 11]
        block = [lines[index].strip() for index in indices]
        first_load = re.fullmatch(r"lw\s+\$2,([A-Za-z_][A-Za-z0-9_]*)", block[0])
        second_load = re.fullmatch(r"lw\s+\$4,([A-Za-z_][A-Za-z0-9_]*)", block[7])
        call = re.fullmatch(r"jal\s+([A-Za-z_][A-Za-z0-9_]*)", block[9])
        if first_load is None or second_load is None or call is None:
            continue
        if first_load.group(1) != second_load.group(1):
            continue
        if block[1:7] != [
            "lw\t$3,0($5)", "lw\t$4,4($5)", "lw\t$6,8($5)",
            "sw\t$3,124($2)", "sw\t$4,128($2)", "sw\t$6,132($2)",
        ]:
            continue
        if block[8] != "addu\t$6,$sp,16" or block[10] != "addu\t$4,$4,40":
            continue
        symbol = first_load.group(1)
        callee = call.group(1)
        replacement = [
            f"lw\t$2,{symbol}",
            "nop",
            "addu\t$4,$2,$zero",
            "lw\t$3,0($5)",
            "lw\t$6,4($5)",
            "lw\t$7,8($5)",
            "sw\t$3,124($2)",
            "sw\t$6,128($2)",
            "sw\t$7,132($2)",
            "addu\t$4,$4,40",
            f"jal\t{callee}",
            "addu\t$6,$sp,16",
        ]
        lines[indices[0]:indices[-1] + 1] = replacement
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX vector-copy call-setup pattern not found")


def reuse_aspsx_initialized_zero(text):
    """Use an initialized zero-valued register instead of the hardware zero."""
    lines = text.splitlines()
    meaningful = [
        index for index, line in enumerate(lines)
        if line.strip() and not line.lstrip().startswith("#")
    ]
    for offset in range(len(meaningful) - 6):
        indices = meaningful[offset:offset + 7]
        block = [lines[index].strip() for index in indices]
        if block[0] != "addu\t$2,$0,$zero":
            continue
        if block[1] != "lb\t$3,160($5)" or not block[2].startswith("nop"):
            continue
        if not re.fullmatch(r"bne\s+\$3,\$0,\$L\d+", block[3]):
            continue
        if not re.fullmatch(r"li\s+\$7,0x00000001(?:\s+# 1)?", block[4]):
            continue
        if not re.fullmatch(r"j\s+\$L\d+", block[5]):
            continue
        if block[6] != "addu\t$7,$0,$zero":
            continue
        lines[indices[6]] = "addu\t$7,$2,$zero"
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX initialized-zero reuse pattern not found")


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
        if not isinstance(unit.get("aspsx_hoist_small_data_load", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_hoist_small_data_load must be boolean"
            )
        if not isinstance(unit.get("aspsx_reorder_jump_store", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reorder_jump_store must be boolean"
            )
        if not isinstance(unit.get("aspsx_reorder_argument_saves", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reorder_argument_saves must be boolean"
            )
        if not isinstance(unit.get("aspsx_reuse_load_destination", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reuse_load_destination must be boolean"
            )
        if not isinstance(unit.get("aspsx_fill_load_delay", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_fill_load_delay must be boolean"
            )
        if not isinstance(unit.get("aspsx_reorder_vector_copy_call", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reorder_vector_copy_call must be boolean"
            )
        if not isinstance(unit.get("aspsx_reuse_zero_value", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reuse_zero_value must be boolean"
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
    if unit.get("aspsx_hoist_small_data_load", False):
        processed.write_text(
            hoist_aspsx_small_data_load_before_prologue(processed.read_text())
        )
    if unit.get("aspsx_reorder_jump_store", False):
        processed.write_text(
            reorder_aspsx_jump_store_delay_slot(processed.read_text())
        )
    if unit.get("aspsx_reorder_argument_saves", False):
        processed.write_text(
            reorder_aspsx_argument_save_groups(processed.read_text())
        )
    if unit.get("aspsx_reuse_load_destination", False):
        processed.write_text(
            reuse_aspsx_symbolic_load_destination(processed.read_text())
        )
    if unit.get("aspsx_fill_load_delay", False):
        processed.write_text(
            fill_aspsx_symbolic_load_delay_slot(processed.read_text())
        )
    if unit.get("aspsx_reorder_vector_copy_call", False):
        processed.write_text(
            reorder_aspsx_vector_copy_call_setup(processed.read_text())
        )
    if unit.get("aspsx_reuse_zero_value", False):
        processed.write_text(
            reuse_aspsx_initialized_zero(processed.read_text())
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
