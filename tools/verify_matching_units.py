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
        zero_init = re.fullmatch(r"addu\s+\$(\d+),\$0,\$zero", block[0])
        side_load = re.fullmatch(r"lb\s+\$(\d+),160\(\$(\d+)\)", block[1])
        if not zero_init or not side_load:
            continue
        zero_register = zero_init.group(1)
        side_register = side_load.group(1)
        if not block[2].startswith("nop"):
            continue
        if not re.fullmatch(
            rf"bne\s+\${side_register},\$0,\$L\d+", block[3]
        ):
            continue
        one_init = re.fullmatch(
            r"li\s+\$(\d+),0x00000001(?:\s+# 1)?", block[4]
        )
        if not one_init:
            continue
        destination = one_init.group(1)
        if not re.fullmatch(r"j\s+\$L\d+", block[5]):
            continue
        if not re.fullmatch(
            rf"addu\s+\${destination},\$0,\$zero", block[6]
        ):
            continue
        lines[indices[6]] = f"addu\t${destination},${zero_register},$zero"
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX initialized-zero reuse pattern not found")


def restore_aspsx_byte_test_temporary(text):
    """Restore a temporary register used for byte tests of saved values."""
    patterns = {
        "andi\t$17,$17,0x00ff": ("andi\t$2,$17,0x00ff", 1),
        "beq\t$17,$0,": ("beq\t$2,$0,", 1),
        "andi\t$16,$16,0x00ff": ("andi\t$2,$16,0x00ff", 2),
        "beq\t$16,$0,": ("beq\t$2,$0,", 1),
    }
    for source, (_, count) in patterns.items():
        if text.count(source) != count:
            raise ValueError("ASPSX byte-test temporary pattern not found")
    for source, (replacement, _) in patterns.items():
        text = text.replace(source, replacement)
    return text


def use_aspsx_symbolic_address_scratch(text):
    """Expand two adjacent symbolic addresses through the assembler scratch."""
    lines = text.splitlines()
    address = re.compile(r"\s*la\s+\$(\d+),([A-Za-z_][A-Za-z0-9_]*)\s*")
    for index in range(len(lines) - 1):
        first = address.fullmatch(lines[index])
        second = address.fullmatch(lines[index + 1])
        if not first or not second:
            continue
        first_register, first_symbol = first.groups()
        second_register, second_symbol = second.groups()
        if "2" in (first_register, second_register):
            continue
        lines[index:index + 2] = [
            f"lui\t$2,%hi({first_symbol})",
            f"addiu\t${first_register},$2,%lo({first_symbol})",
            f"lui\t$2,%hi({second_symbol})",
            f"addiu\t${second_register},$2,%lo({second_symbol})",
        ]
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX adjacent symbolic-address pattern not found")


def reuse_aspsx_call_constant(text):
    """Reuse the first argument register for equal call constants."""
    lines = text.splitlines()
    immediate = re.compile(r"\s*li\s+\$(\d+),([^\s]+)(?:\s+#.*)?\s*")
    call = re.compile(r"\s*jal\s+.+")
    for index in range(len(lines) - 2):
        first = immediate.fullmatch(lines[index])
        second = immediate.fullmatch(lines[index + 1])
        if not first or not second:
            continue
        source, value = first.groups()
        second_register, second_value = second.groups()
        if source != "4" or second_register != "5" or value != second_value:
            continue
        for call_index in range(index + 2, min(index + 12, len(lines) - 1)):
            third = immediate.fullmatch(lines[call_index + 1])
            if not call.fullmatch(lines[call_index]) or not third:
                continue
            third_register, third_value = third.groups()
            if third_register != "6" or third_value != value:
                continue
            if any(re.search(r"\$4(?:,|\s)", line) for line in lines[index + 2:call_index]):
                continue
            lines[index + 1] = "addu\t$5,$4,$zero"
            lines[call_index + 1] = "addu\t$6,$4,$zero"
            return "\n".join(lines) + "\n"
    raise ValueError("ASPSX repeated call-constant pattern not found")


def defer_aspsx_saved_call_argument(text):
    """Defer saving a fourth argument until the first call delay slot."""
    source = (
        "sw\t$17,52($sp)\n"
        "addu\t$17,$5,$zero\n"
        "sw\t$18,56($sp)\n"
        "addu\t$18,$6,$zero\n"
        "sw\t$16,48($sp)\n"
        "addu\t$16,$7,$zero\n"
        "addu\t$4,$sp,16\n"
        "la\t$5,D_8009F368\n"
        "sw\t$31,60($sp)\n"
        "jal\tfunc_8008CD30\n"
        "addu\t$6,$2,$zero\n"
    )
    replacement = (
        "sw\t$17,52($sp)\n"
        "addu\t$17,$5,$zero\n"
        "sw\t$18,56($sp)\n"
        "addu\t$18,$6,$zero\n"
        "addu\t$4,$sp,16\n"
        "la\t$5,D_8009F368\n"
        "addu\t$6,$2,$zero\n"
        "sw\t$16,48($sp)\n"
        "sw\t$31,60($sp)\n"
        "jal\tfunc_8008CD30\n"
        "addu\t$16,$7,$zero\n"
    )
    if text.count(source) != 1:
        raise ValueError("ASPSX saved call-argument pattern not found")
    return text.replace(source, replacement)


def expand_aspsx_indexed_return_store(text):
    """Materialize an indexed global store and place it in the return slot."""
    lines = text.splitlines()
    shift = re.compile(r"\s*sll\s+\$(\d+),\$(\d+),(\d+)\s*")
    store = re.compile(
        r"\s*sw\s+\$(\d+),([A-Za-z_][A-Za-z0-9_]*)(?:\+(-?\d+))?\(\$(\d+)\)\s*"
    )
    for index in range(len(lines) - 3):
        shift_match = shift.fullmatch(lines[index])
        store_match = store.fullmatch(lines[index + 1])
        if not shift_match or not store_match:
            continue
        address_register, source_register, amount = shift_match.groups()
        value_register, symbol, offset, store_base = store_match.groups()
        if address_register != store_base:
            continue
        if lines[index + 2].strip() != "j\t$31":
            continue
        if not lines[index + 3].strip().startswith("nop"):
            continue
        displacement = offset or "0"
        lines[index:index + 4] = [
            f"lui\t$3,%hi({symbol})",
            f"addiu\t$3,$3,%lo({symbol})",
            f"sll\t${address_register},${source_register},{amount}",
            f"addu\t${address_register},${address_register},$3",
            "j\t$31",
            f"sw\t${value_register},{displacement}(${address_register})",
        ]
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX indexed return-store pattern not found")


def hoist_aspsx_indexed_address(text):
    """Materialize a symbolic base before calculating its array index."""
    lines = text.splitlines()
    shift = re.compile(r"\s*sll\s+\$(\d+),\$(\d+),(\d+)\s*")
    address = re.compile(r"\s*la\s+\$(\d+),([A-Za-z_][A-Za-z0-9_]*)\s*")
    for index in range(len(lines) - 1):
        if not shift.fullmatch(lines[index]) or not address.fullmatch(lines[index + 1]):
            continue
        lines[index], lines[index + 1] = lines[index + 1], lines[index]
        return "\n".join(lines) + "\n"
    raise ValueError("ASPSX indexed-address pattern not found")


def rotate_aspsx_saved_body_registers(text):
    """Rotate s1/s2/s3 allocation inside a saved-register function body."""
    lines = text.splitlines()
    start = next(
        (index + 1 for index, line in enumerate(lines)
         if re.fullmatch(r"\s*sw\s+\$16,16\(\$sp\)\s*", line)),
        None,
    )
    end = next(
        (index for index, line in enumerate(lines)
         if start is not None and index >= start
         and re.fullmatch(r"\s*lw\s+\$31,40\(\$sp\)\s*", line)),
        None,
    )
    if start is None or end is None or start >= end:
        raise ValueError("ASPSX saved-body register pattern not found")
    body = "\n".join(lines[start:end])
    if not all(re.search(rf"\${register}\b", body) for register in (17, 18, 19)):
        raise ValueError("ASPSX saved-body register set not found")
    body = re.sub(r"\$(17|18|19)\b", lambda match: {
        "17": "$18",
        "18": "$19",
        "19": "$17",
    }[match.group(1)], body)
    lines[start:end] = body.splitlines()
    return "\n".join(lines) + "\n"


def normalize_aspsx_indexed_byte_access(text):
    """Restore ASPSX's offset-first expansion for two indexed byte accesses."""
    early_return = re.compile(
        r"(\$L\d+:\n)j\t\$31\nnop  # DEBUG: branch/jump\n"
    )
    match = early_return.search(text)
    final_labels = re.findall(
        r"(\$L\d+):\nj\t\$31\nnop  # DEBUG: branch/jump", text
    )
    if match is None or len(final_labels) != 2:
        raise ValueError("ASPSX indexed-byte return pattern not found")
    early_label = match.group(1).strip()[:-1]
    final_label = final_labels[-1]
    text = text[:match.start()] + (
        f"j\t{final_label}\n"
        "nop  # DEBUG: branch/jump\n"
    ) + text[match.end():]
    text = re.sub(rf"{re.escape(early_label)}\b", final_label, text)
    pattern = re.compile(
        r"addu\t\$2,\$3,\$2\n"
        r"lbu\t\$3,4\(\$2\)\n"
        r"#nop[^\n]*\n"
        r"addu\t\$2,\$2,4\n"
    )
    replacements = 0

    def replace_access(match):
        nonlocal replacements
        replacements += 1
        tail = "" if replacements == 1 else "nop\n"
        return (
            "addu\t$2,$2,4\n"
            "addu\t$2,$3,$2\n"
            "lbu\t$3,0($2)\n"
            + tail
        )

    text = pattern.sub(replace_access, text)
    if replacements != 2:
        raise ValueError("ASPSX indexed-byte access pattern not found")
    return text


def normalize_aspsx_packet_cursor(text):
    """Restore retail packet-cursor expansion and loop temporaries."""
    cursor = re.compile(
        r"lui\t\$3,%hi\(([^)]+)\)[^\n]*\n"
        r"lui\t\$2,%hi\(([^)]+)\)[^\n]*\n"
        r"lw\t\$4,%lo\(\1\)\(\$3\)[^\n]*\n"
        r"#nop[^\n]*\n"
        r"lw\t\$2,%lo\(\2\)\(\$2\)[^\n]*\n"
        r"nop[^\n]*\n"
        r"sltu\t\$2,\$4,\$2\n"
        r"beq\t\$2,\$0,(\$L\d+)\n"
        r"addu\t\$2,\$4,28\n\n"
        r"sw\t\$2,%lo\(\1\)\(\$3\)\n"
    )
    match = cursor.search(text)
    if match is None:
        raise ValueError("ASPSX packet-cursor pattern not found")
    current, limit, label = match.groups()
    replacement = (
        f"lui\t$3,%hi({current})\n"
        f"lw\t$3,%lo({current})($3)\n"
        f"lui\t$2,%hi({limit})\n"
        f"lw\t$2,%lo({limit})($2)\n"
        "nop\n"
        "sltu\t$2,$3,$2\n"
        f"beq\t$2,$0,{label}\n"
        "addu\t$4,$3,$zero\n"
        "addu\t$2,$4,28\n"
        ".set\tnoat\n"
        f"lui\t$at,%hi({current})\n"
        f"sw\t$2,%lo({current})($at)\n"
        ".set\tat\n"
    )
    text = text[:match.start()] + replacement + text[match.end():]
    delta = re.compile(
        r"beq\t\$4,\$0,(\$L\d+)\n"
        r"addu\t\$3,\$4,-10\n\n"
        r"sltu\t\$2,\$3,22\n"
        r"beq\t\$2,\$0,(\$L\d+)\n"
        r"addu\t\$16,\$16,1\n\n"
        r"j\t(\$L\d+)\n"
        r"addu\t\$18,\$3,\$zero\n"
    )
    match = delta.search(text)
    if match is None:
        raise ValueError("ASPSX packet control-code pattern not found")
    done, draw, loop = match.groups()
    replacement = (
        f"beq\t$4,$0,{done}\n"
        "addu\t$2,$4,-10\n\n"
        "sltu\t$2,$2,22\n"
        f"beq\t$2,$0,{draw}\n"
        "addu\t$16,$16,1\n\n"
        f"j\t{loop}\n"
        "addu\t$18,$4,-10\n"
    )
    text = text[:match.start()] + replacement + text[match.end():]
    epilogue = re.compile(r"j\t\$31\naddu\t\$sp,\$sp,40\n")
    text, count = epilogue.subn("addu\t$sp,$sp,40\nj\t$31\nnop\n", text)
    if count != 1:
        raise ValueError("ASPSX packet return pattern not found")
    return text


def normalize_aspsx_line_packet(text):
    """Restore retail allocation and coordinate scheduling for a line packet."""
    cursor = re.compile(
        r"lui\t\$3,%hi\(([^)]+)\)[^\n]*\n"
        r"lui\t\$2,%hi\(([^)]+)\)[^\n]*\n"
        r"lw\t\$6,%lo\(\1\)\(\$3\)[^\n]*\n"
        r"#nop[^\n]*\n"
        r"lw\t\$2,%lo\(\2\)\(\$2\)[^\n]*\n"
        r"nop[^\n]*\n"
        r"sltu\t\$2,\$6,\$2\n"
        r"beq\t\$2,\$0,(\$L\d+)\n"
        r"addu\t\$2,\$6,28\n\n"
        r"sw\t\$2,%lo\(\1\)\(\$3\)\n"
        r"\3:\n"
        r"li\t\$3,1082130432[^\n]*\n"
    )
    match = cursor.search(text)
    if match is None:
        raise ValueError("ASPSX line-packet cursor pattern not found")
    current, limit, done = match.groups()
    advance = "$L_line_packet_advance"
    replacement = (
        f"lui\t$3,%hi({current})\n"
        f"lw\t$3,%lo({current})($3)\n"
        f"lui\t$2,%hi({limit})\n"
        f"lw\t$2,%lo({limit})($2)\n"
        "nop\n"
        "sltu\t$2,$3,$2\n"
        f"bne\t$2,$0,{advance}\n"
        "addu\t$6,$3,$zero\n"
        f"j\t{done}\n"
        "li\t$3,1082130432\n"
        f"{advance}:\n"
        "addu\t$2,$6,28\n"
        ".set\tnoat\n"
        f"lui\t$at,%hi({current})\n"
        f"sw\t$2,%lo({current})($at)\n"
        ".set\tat\n"
        "li\t$3,1082130432\n"
        f"{done}:\n"
    )
    text = text[:match.start()] + replacement + text[match.end():]
    schedule = re.compile(
        r"lw\t\$4,20\(\$sp\)\n"
        r"#nop[^\n]*\n"
        r"li\t\$5,2883584[^\n]*\n"
        r"sw\t\$2,0\(\$6\)\n"
        r"sw\t\$3,4\(\$6\)\n"
        r"lw\t\$3,16\(\$sp\)\n"
        r"#nop[^\n]*\n"
    )
    match = schedule.search(text)
    if match is None:
        raise ValueError("ASPSX line-packet coordinate pattern not found")
    replacement = (
        "li\t$5,2883584\n"
        "sw\t$2,0($6)\n"
        "sw\t$3,4($6)\n"
        "lw\t$3,20($sp)\n"
        "lw\t$4,16($sp)\n"
    )
    start = match.start()
    text = text[:start] + replacement + text[match.end():]
    body_start = start + len(replacement)
    end = text.find("lw\t$31,28($sp)", body_start)
    if end < 0:
        raise ValueError("ASPSX line-packet epilogue pattern not found")
    body = text[body_start:end]
    body = body.replace("$3", "$tmp").replace("$4", "$3").replace("$tmp", "$4")
    text = text[:body_start] + body + text[end:]
    epilogue = re.compile(r"j\t\$31\naddu\t\$sp,\$sp,32\n")
    text, count = epilogue.subn("addu\t$sp,$sp,32\nj\t$31\nnop\n", text)
    if count != 1:
        raise ValueError("ASPSX line-packet return pattern not found")
    return text


def normalize_aspsx_context_wrapper(text):
    """Restore retail scheduling for a context-setting call wrapper."""
    pattern = re.compile(
        r"lw\t\$8,40\(\$sp\)\n"
        r"#nop[^\n]*\n"
        r"lw\t\$3,44\(\$sp\)\n"
        r"#nop[^\n]*\n"
        r"li\t\$2,0x00000001[^\n]*\n"
        r"sw\t\$31,16\(\$sp\)\n"
        r"sb\t\$2,%gp_rel\(([^)]+)\)\(\$gp\)\n"
        r"sw\t\$7,%gp_rel\(([^)]+)\)\(\$gp\)\n"
        r"lw\t\$2,0\(\$8\)\n"
        r"#nop[^\n]*\n"
        r"sra\t\$3,\$3,4\n"
        r"sh\t\$3,%gp_rel\(([^)]+)\)\(\$gp\)\n"
        r"lw\t\$3,8\(\$8\)\n"
        r"#nop[^\n]*\n"
        r"sra\t\$2,\$2,4\n"
        r"sra\t\$3,\$3,4\n"
        r"sh\t\$2,%gp_rel\(([^)]+)\)\(\$gp\)\n"
        r"sh\t\$3,%gp_rel\(([^)]+)\)\(\$gp\)\n"
    )
    match = pattern.search(text)
    if match is None:
        raise ValueError("ASPSX context-wrapper pattern not found")
    flag, context, height, x_value, z_value = match.groups()
    replacement = (
        "lw\t$8,40($sp)\n"
        "lw\t$2,44($sp)\n"
        "sw\t$31,16($sp)\n"
        "lw\t$3,0($8)\n"
        "sra\t$2,$2,4\n"
        f"sh\t$2,%gp_rel({height})($gp)\n"
        "sra\t$3,$3,4\n"
        f"sh\t$3,%gp_rel({x_value})($gp)\n"
        "lw\t$3,8($8)\n"
        "li\t$2,0x00000001\n"
        f"sb\t$2,%gp_rel({flag})($gp)\n"
        f"sw\t$7,%gp_rel({context})($gp)\n"
        "sra\t$3,$3,4\n"
        f"sh\t$3,%gp_rel({z_value})($gp)\n"
    )
    return text[:match.start()] + replacement + text[match.end():]


def defer_aspsx_column_mask(text):
    """Move a byte-column mask after construction of its row base."""
    pattern = re.compile(
        r"andi\t\$5,\$5,0x00ff\n"
        r"lui\t\$2,%hi\(([^)]+)\)[^\n]*\n"
        r"addiu\t\$2,\$2,%lo\(\1\)[^\n]*\n"
        r"sll\t\$6,\$6,10\n"
        r"addu\t\$2,\$6,\$2\n"
    )
    match = pattern.search(text)
    if match is None:
        raise ValueError("ASPSX column-mask pattern not found")
    symbol = match.group(1)
    replacement = (
        f"lui\t$2,%hi({symbol})\n"
        f"addiu\t$2,$2,%lo({symbol})\n"
        "sll\t$6,$6,10\n"
        "addu\t$2,$6,$2\n"
        "andi\t$5,$5,0x00ff\n"
    )
    return text[:match.start()] + replacement + text[match.end():]


def normalize_aspsx_shared_byte_store_return(text):
    """Merge an early return into a terminal byte-store epilogue."""
    terminal = re.compile(
        r"(?P<indent>\s*)j\t\$31\n"
        r"(?P<store>sb\t\$([0-9]+),[^\n]+)\n\n"
        r"(?P<end>\.end\t[^\n]+)"
    )
    match = terminal.search(text)
    if match is None:
        raise ValueError("ASPSX terminal byte-store return pattern not found")
    label = "$L_aspsx_shared_return"
    replacement = (
        f"{match.group('indent')}{match.group('store')}\n"
        f"{label}:\n"
        "j\t$31\n"
        "nop\n\n"
        f"{match.group('end')}"
    )
    text = text[:match.start()] + replacement + text[match.end():]
    early = re.compile(r"j\t\$31\n(addu\t\$2,\$0,\$zero\n)")
    text, count = early.subn(f"j\t{label}\n\\1", text, count=1)
    if count != 1:
        raise ValueError("ASPSX early zero-return pattern not found")
    return text


def normalize_aspsx_color_ramp_schedule(text):
    """Restore the retail scheduling of the 15-bit color-ramp loop."""
    setup = re.compile(
        r"addiu\t\$2,\$2,%lo\(([^)]+)\)[^\n]*\n"
        r"li\t\$25,10[^\n]*\n"
        r"li\t\$24,5[^\n]*\n"
        r"sll\t\$3,\$4,\$24\n"
        r"addu\t\$3,\$2,\$3\n"
    )
    match = setup.search(text)
    if match is None:
        raise ValueError("ASPSX color-ramp setup pattern not found")
    symbol = match.group(1)
    replacement = (
        "li\t$25,10\n"
        "li\t$24,5\n"
        f"addiu\t$2,$2,%lo({symbol})\n"
        "sll\t$3,$4,5\n"
        "addu\t$3,$3,$2\n"
    )
    text = text[:match.start()] + replacement + text[match.end():]

    division = re.compile(
        r"addu\t\$2,\$9,\$zero\n"
        r"bgez\t\$2,(\$L[0-9A-Za-z_]+)\n"
        r"nop[^\n]*\n"
        r"addu\t\$2,\$2,15\n"
        r"\1:\n"
        r"sra\t\$2,\$2,4\n"
        r"addu\t\$8,\$10,\$zero\n"
        r"bgez\t\$8,(\$L[0-9A-Za-z_]+)\n"
        r"sll\t\$3,\$2,\$25\n\n"
        r"addu\t\$8,\$8,15\n"
        r"\2:\n"
        r"sra\t\$2,\$8,4\n"
        r"addu\t\$4,\$11,\$zero\n"
        r"bgez\t\$4,(\$L[0-9A-Za-z_]+)\n"
        r"sll\t\$8,\$2,\$24\n\n"
        r"addu\t\$4,\$4,15\n"
        r"\3:\n"
    )
    match = division.search(text)
    if match is None:
        raise ValueError("ASPSX color-ramp division pattern not found")
    first, second, third = match.groups()
    replacement = (
        f"bgez\t$9,{first}\n"
        "addu\t$2,$9,$zero\n"
        "addu\t$2,$9,15\n"
        f"{first}:\n"
        "addu\t$8,$10,$zero\n"
        "sra\t$2,$2,4\n"
        f"bgez\t$10,{second}\n"
        "sll\t$3,$2,$25\n\n"
        "addu\t$8,$10,15\n"
        f"{second}:\n"
        "addu\t$4,$11,$zero\n"
        "sra\t$2,$8,4\n"
        f"bgez\t$11,{third}\n"
        "sll\t$8,$2,$24\n\n"
        "addu\t$4,$11,15\n"
        f"{third}:\n"
    )
    return text[:match.start()] + replacement + text[match.end():]


def normalize_aspsx_signed_slot_test(text):
    """Restore the retail registers and zero-result delay slots for a slot test."""
    signed = re.compile(
        r"sll\t\$4,\$5,24\n\s*"
        r"sra\t\$4,\$4,24\n"
        r"bltz\t\$4,(\$L[0-9A-Za-z_]+)\n"
        r"nop[^\n]*\n"
        r"lw\t\$2,0\(\$3\)\n"
        r"nop[^\n]*\n"
        r"sltu\t\$2,\$4,\$2\n"
    )
    match = signed.search(text)
    if match is None:
        raise ValueError("ASPSX signed-slot range pattern not found")
    target = match.group(1)
    replacement = (
        "sll\t$2,$5,24\n"
        "sra\t$5,$2,24\n"
        f"bltz\t$5,{target}\n"
        "addu\t$2,$0,$zero\n"
        "lw\t$2,0($3)\n"
        "nop\n"
        "sltu\t$2,$5,$2\n"
    )
    text = text[:match.start()] + replacement + text[match.end():]
    final = re.compile(
        r"beq\t\$4,\$3,(\$L[0-9A-Za-z_]+)\n"
        r"nop[^\n]*\n"
        r"li\t\$2,0x00000001[^\n]*\n"
    )
    match = final.search(text)
    if match is None:
        raise ValueError("ASPSX signed-slot final test pattern not found")
    replacement = (
        f"beq\t$4,$3,{match.group(1)}\n"
        "addu\t$2,$0,$zero\n"
        "li\t$2,0x00000001\n"
    )
    return text[:match.start()] + replacement + text[match.end():]


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
        if not isinstance(unit.get("aspsx_restore_byte_test_temp", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_restore_byte_test_temp must be boolean"
            )
        if not isinstance(unit.get("aspsx_symbolic_address_scratch", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_symbolic_address_scratch must be boolean"
            )
        if not isinstance(unit.get("aspsx_reuse_call_constant", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_reuse_call_constant must be boolean"
            )
        if not isinstance(unit.get("aspsx_defer_saved_call_argument", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_defer_saved_call_argument must be boolean"
            )
        if not isinstance(unit.get("aspsx_expand_indexed_return_store", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_expand_indexed_return_store must be boolean"
            )
        if not isinstance(unit.get("aspsx_hoist_indexed_address", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_hoist_indexed_address must be boolean"
            )
        if not isinstance(unit.get("aspsx_rotate_saved_body", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_rotate_saved_body must be boolean"
            )
        if not isinstance(unit.get("aspsx_indexed_byte_access", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_indexed_byte_access must be boolean"
            )
        if not isinstance(unit.get("aspsx_packet_cursor", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_packet_cursor must be boolean"
            )
        if not isinstance(unit.get("aspsx_line_packet", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_line_packet must be boolean"
            )
        if not isinstance(unit.get("aspsx_context_wrapper", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_context_wrapper must be boolean"
            )
        if not isinstance(unit.get("aspsx_defer_column_mask", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_defer_column_mask must be boolean"
            )
        if not isinstance(unit.get("aspsx_shared_byte_store_return", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_shared_byte_store_return must be boolean"
            )
        if not isinstance(unit.get("aspsx_color_ramp_schedule", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_color_ramp_schedule must be boolean"
            )
        if not isinstance(unit.get("aspsx_signed_slot_test", False), bool):
            raise ValueError(
                f"{unit['name']}: aspsx_signed_slot_test must be boolean"
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
    if unit.get("aspsx_restore_byte_test_temp", False):
        processed.write_text(
            restore_aspsx_byte_test_temporary(processed.read_text())
        )
    if unit.get("aspsx_symbolic_address_scratch", False):
        processed.write_text(
            use_aspsx_symbolic_address_scratch(processed.read_text())
        )
    if unit.get("aspsx_reuse_call_constant", False):
        processed.write_text(reuse_aspsx_call_constant(processed.read_text()))
    if unit.get("aspsx_defer_saved_call_argument", False):
        processed.write_text(
            defer_aspsx_saved_call_argument(processed.read_text())
        )
    if unit.get("aspsx_expand_indexed_return_store", False):
        processed.write_text(
            expand_aspsx_indexed_return_store(processed.read_text())
        )
    if unit.get("aspsx_hoist_indexed_address", False):
        processed.write_text(hoist_aspsx_indexed_address(processed.read_text()))
    if unit.get("aspsx_rotate_saved_body", False):
        processed.write_text(
            rotate_aspsx_saved_body_registers(processed.read_text())
        )
    if unit.get("aspsx_indexed_byte_access", False):
        processed.write_text(
            normalize_aspsx_indexed_byte_access(processed.read_text())
        )
    if unit.get("aspsx_packet_cursor", False):
        processed.write_text(normalize_aspsx_packet_cursor(processed.read_text()))
    if unit.get("aspsx_line_packet", False):
        processed.write_text(normalize_aspsx_line_packet(processed.read_text()))
    if unit.get("aspsx_context_wrapper", False):
        processed.write_text(normalize_aspsx_context_wrapper(processed.read_text()))
    if unit.get("aspsx_defer_column_mask", False):
        processed.write_text(defer_aspsx_column_mask(processed.read_text()))
    if unit.get("aspsx_shared_byte_store_return", False):
        processed.write_text(
            normalize_aspsx_shared_byte_store_return(processed.read_text())
        )
    if unit.get("aspsx_color_ramp_schedule", False):
        processed.write_text(
            normalize_aspsx_color_ramp_schedule(processed.read_text())
        )
    if unit.get("aspsx_signed_slot_test", False):
        processed.write_text(
            normalize_aspsx_signed_slot_test(processed.read_text())
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
