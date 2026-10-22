import unittest

from tools.verify_matching_units import (
    defer_aspsx_saved_call_argument,
    expand_aspsx_indexed_return_store,
    hoist_aspsx_indexed_address,
    fill_aspsx_symbolic_load_delay_slot,
    hoist_aspsx_small_data_load_before_prologue,
    move_aspsx_load_delay_label,
    normalize_aspsx_indexed_byte_access,
    reorder_aspsx_argument_save_groups,
    reorder_aspsx_vector_copy_call_setup,
    rotate_aspsx_saved_body_registers,
    reuse_aspsx_initialized_zero,
    restore_aspsx_byte_test_temporary,
    reuse_aspsx_call_constant,
    reuse_aspsx_symbolic_load_destination,
    reorder_aspsx_return_delay_slot,
    reorder_aspsx_jump_store_delay_slot,
    use_aspsx_symbolic_address_scratch,
)


class AspsxReorderTests(unittest.TestCase):
    def test_normalizes_indexed_byte_access_and_shared_return(self):
        source = (
            "beq\t$2,$0,$L10\n"
            "$L10:\n"
            "j\t$31\n"
            "nop  # DEBUG: branch/jump\n"
            "addu\t$2,$3,$2\n"
            "lbu\t$3,4($2)\n"
            "#nop # load delay\n"
            "addu\t$2,$2,4\n"
            "j\t$L8\n"
            "andi\t$3,$3,0x00fd\n"
            "addu\t$2,$3,$2\n"
            "lbu\t$3,4($2)\n"
            "#nop # load delay\n"
            "addu\t$2,$2,4\n"
            "ori\t$3,$3,0x0002\n"
            "$L9:\n"
            "j\t$31\n"
            "nop  # DEBUG: branch/jump\n"
        )
        expected = (
            "beq\t$2,$0,$L9\n"
            "j\t$L9\n"
            "nop  # DEBUG: branch/jump\n"
            "addu\t$2,$2,4\n"
            "addu\t$2,$3,$2\n"
            "lbu\t$3,0($2)\n"
            "j\t$L8\n"
            "andi\t$3,$3,0x00fd\n"
            "addu\t$2,$2,4\n"
            "addu\t$2,$3,$2\n"
            "lbu\t$3,0($2)\n"
            "nop\n"
            "ori\t$3,$3,0x0002\n"
            "$L9:\n"
            "j\t$31\n"
            "nop  # DEBUG: branch/jump\n"
        )
        self.assertEqual(normalize_aspsx_indexed_byte_access(source), expected)

    def test_rotates_saved_registers_only_inside_function_body(self):
        source = (
            "sw\t$19,28($sp)\n"
            "sw\t$18,24($sp)\n"
            "sw\t$17,20($sp)\n"
            "sw\t$16,16($sp)\n"
            "addu\t$17,$2,$zero\n"
            "addu\t$18,$17,$19\n"
            "lw\t$31,40($sp)\n"
            "lw\t$19,28($sp)\n"
            "lw\t$18,24($sp)\n"
            "lw\t$17,20($sp)\n"
        )
        expected = (
            "sw\t$19,28($sp)\n"
            "sw\t$18,24($sp)\n"
            "sw\t$17,20($sp)\n"
            "sw\t$16,16($sp)\n"
            "addu\t$18,$2,$zero\n"
            "addu\t$19,$18,$17\n"
            "lw\t$31,40($sp)\n"
            "lw\t$19,28($sp)\n"
            "lw\t$18,24($sp)\n"
            "lw\t$17,20($sp)\n"
        )
        self.assertEqual(rotate_aspsx_saved_body_registers(source), expected)

    def test_hoists_symbolic_base_before_array_index(self):
        source = "sll\t$3,$3,3\nla\t$2,D_table\naddu\t$3,$3,$2\n"
        expected = "la\t$2,D_table\nsll\t$3,$3,3\naddu\t$3,$3,$2\n"
        self.assertEqual(hoist_aspsx_indexed_address(source), expected)

    def test_expands_indexed_store_into_return_slot(self):
        source = (
            "sll\t$2,$7,4\n"
            "sw\t$6,D_table+12($2)\n"
            "j\t$31\n"
            "nop  # DEBUG: branch/jump\n"
        )
        expected = (
            "lui\t$3,%hi(D_table)\n"
            "addiu\t$3,$3,%lo(D_table)\n"
            "sll\t$2,$7,4\n"
            "addu\t$2,$2,$3\n"
            "j\t$31\n"
            "sw\t$6,12($2)\n"
        )
        self.assertEqual(expand_aspsx_indexed_return_store(source), expected)

    def test_defers_saved_fourth_call_argument(self):
        source = (
            "sw\t$17,52($sp)\naddu\t$17,$5,$zero\n"
            "sw\t$18,56($sp)\naddu\t$18,$6,$zero\n"
            "sw\t$16,48($sp)\naddu\t$16,$7,$zero\n"
            "addu\t$4,$sp,16\nla\t$5,D_8009F368\n"
            "sw\t$31,60($sp)\njal\tfunc_8008CD30\n"
            "addu\t$6,$2,$zero\n"
        )
        result = defer_aspsx_saved_call_argument(source)
        self.assertIn("addu\t$6,$2,$zero\nsw\t$16,48($sp)", result)
        self.assertTrue(result.endswith("jal\tfunc_8008CD30\naddu\t$16,$7,$zero\n"))

    def test_uses_symbolic_address_scratch_for_adjacent_addresses(self):
        source = "la\t$4,left\nla\t$3,right\nli\t$2,1\n"
        expected = (
            "lui\t$2,%hi(left)\n"
            "addiu\t$4,$2,%lo(left)\n"
            "lui\t$2,%hi(right)\n"
            "addiu\t$3,$2,%lo(right)\n"
            "li\t$2,1\n"
        )
        self.assertEqual(use_aspsx_symbolic_address_scratch(source), expected)

    def test_reuses_equal_call_constants(self):
        source = (
            "li\t$4,0x0000001f\t\t# 31\n"
            "li\t$5,0x0000001f\t\t# 31\n"
            "sw\t$0,value\n"
            "jal\tfunction\n"
            "li\t$6,0x0000001f\t\t# 31\n"
        )
        expected = (
            "li\t$4,0x0000001f\t\t# 31\n"
            "addu\t$5,$4,$zero\n"
            "sw\t$0,value\n"
            "jal\tfunction\n"
            "addu\t$6,$4,$zero\n"
        )
        self.assertEqual(reuse_aspsx_call_constant(source), expected)

    def test_restores_byte_test_temporary(self):
        source = (
            "andi\t$17,$17,0x00ff\nbeq\t$17,$0,$L1\n"
            "andi\t$16,$16,0x00ff\nandi\t$16,$16,0x00ff\n"
            "beq\t$16,$0,$L2\n"
        )
        expected = (
            "andi\t$2,$17,0x00ff\nbeq\t$2,$0,$L1\n"
            "andi\t$2,$16,0x00ff\nandi\t$2,$16,0x00ff\n"
            "beq\t$2,$0,$L2\n"
        )
        self.assertEqual(restore_aspsx_byte_test_temporary(source), expected)

    def test_reuses_initialized_zero_value(self):
        source = (
            "addu\t$2,$0,$zero\nlb\t$3,160($5)\nnop\n"
            "bne\t$3,$0,$L2\nli\t$7,0x00000001\t\t# 1\n"
            "j\t$L3\naddu\t$7,$0,$zero\n$L2:\n"
        )
        expected = source.replace("addu\t$7,$0,$zero", "addu\t$7,$2,$zero")
        self.assertEqual(reuse_aspsx_initialized_zero(source), expected)

    def test_reuses_initialized_zero_with_other_registers(self):
        source = (
            "addu\t$2,$0,$zero\nlb\t$3,160($4)\nnop\n"
            "bne\t$3,$0,$L2\nli\t$6,0x00000001\t\t# 1\n"
            "j\t$L3\naddu\t$6,$0,$zero\n$L2:\n"
        )
        expected = source.replace("addu\t$6,$0,$zero", "addu\t$6,$2,$zero")
        self.assertEqual(reuse_aspsx_initialized_zero(source), expected)

    def test_reorders_vector_copy_call_setup(self):
        source = (
            "lw\t$2,D_camera\n# load delay\nlw\t$3,0($5)\n"
            "lw\t$4,4($5)\nlw\t$6,8($5)\nsw\t$3,124($2)\n"
            "sw\t$4,128($2)\nsw\t$6,132($2)\nlw\t$4,D_camera\n"
            "addu\t$6,$sp,16\njal\ttransform\naddu\t$4,$4,40\n"
        )
        expected = (
            "lw\t$2,D_camera\nnop\naddu\t$4,$2,$zero\n"
            "lw\t$3,0($5)\nlw\t$6,4($5)\nlw\t$7,8($5)\n"
            "sw\t$3,124($2)\nsw\t$6,128($2)\nsw\t$7,132($2)\n"
            "addu\t$4,$4,40\njal\ttransform\naddu\t$6,$sp,16\n"
        )
        self.assertEqual(reorder_aspsx_vector_copy_call_setup(source), expected)

    def test_fills_symbolic_load_delay_with_independent_increment(self):
        source = "sll\t$2,$18,1\naddu\t$18,$18,1\nlw\t$3,D_8009F5B0\nnop\naddu\t$2,$2,$3\n"
        expected = "sll\t$2,$18,1\nlw\t$3,D_8009F5B0\naddu\t$18,$18,1\naddu\t$2,$2,$3\n"
        self.assertEqual(fill_aspsx_symbolic_load_delay_slot(source), expected)

    def test_rejects_dependent_symbolic_load_delay_increment(self):
        with self.assertRaises(ValueError):
            fill_aspsx_symbolic_load_delay_slot("addu\t$3,$3,1\nlw\t$3,D_8009F5B0\nnop\n")
        with self.assertRaises(ValueError):
            fill_aspsx_symbolic_load_delay_slot("addu\t$3,$3,1\nlw\t$2,0($3)\nnop\n")

    def test_hoists_independent_small_data_load_before_prologue(self):
        source = (
            "subu\t$sp,$sp,72\n"
            "sw\t$19,60($sp)\n"
            "lw\t$19,88($sp)\n"
            "sw\t$18,56($sp)\n"
            "addu\t$18,$6,$zero\n"
            "lbu\t$2,%gp_rel(enabled)($gp)\n"
            "sw\t$31,64($sp)\n"
        )
        expected = (
            "lbu\t$2,%gp_rel(enabled)($gp)\n"
            "subu\t$sp,$sp,72\n"
            "sw\t$19,60($sp)\n"
            "lw\t$19,88($sp)\n"
            "sw\t$18,56($sp)\n"
            "addu\t$18,$6,$zero\n"
            "sw\t$31,64($sp)\n"
        )
        self.assertEqual(
            hoist_aspsx_small_data_load_before_prologue(source), expected
        )

    def test_rejects_small_data_load_after_control_flow(self):
        source = (
            "subu\t$sp,$sp,72\n"
            "beq\t$2,$0,$L1\n"
            "lbu\t$2,%gp_rel(enabled)($gp)\n"
        )
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            hoist_aspsx_small_data_load_before_prologue(source)

    def test_reuses_symbolic_load_destination_when_scratch_dies(self):
        source = "lw\t$3,value\n#nop # DEBUG: independent instruction\nli\t$2,2\n"
        expected = (
            "lui\t$3,%hi(value)\n"
            "lw\t$3,%lo(value)($3)\n"
            "#nop # DEBUG: independent instruction\n"
            "li\t$2,2\n"
        )
        self.assertEqual(reuse_aspsx_symbolic_load_destination(source), expected)

    def test_reuses_symbolic_load_destination_before_scratch_reload(self):
        source = (
            "lui\t$2,%hi(value)\n"
            "lw\t$3,%lo(value)($2)\n"
            "nop # DEBUG: load delay\n"
            "lw\t$2,12($3)\n"
        )
        expected = (
            "lui\t$3,%hi(value)\n"
            "lw\t$3,%lo(value)($3)\n"
            "nop # DEBUG: load delay\n"
            "lw\t$2,12($3)\n"
        )
        self.assertEqual(reuse_aspsx_symbolic_load_destination(source), expected)

    def test_keeps_live_symbolic_load_scratch(self):
        source = "lui\t$2,%hi(value)\nlw\t$3,%lo(value)($2)\naddu\t$4,$2,$zero\n"
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            reuse_aspsx_symbolic_load_destination(source)

    def test_reuses_symbolic_address_destination(self):
        source = (
            "lui\t$2,%hi(table) # high\n"
            "addiu\t$16,$2,%lo(table)\n"
            "lw\t$4,0($16)\n"
        )
        expected = (
            "lui\t$16,%hi(table)\n"
            "addiu\t$16,$16,%lo(table)\n"
            "lw\t$4,0($16)\n"
        )
        self.assertEqual(reuse_aspsx_symbolic_load_destination(source), expected)

    def test_orders_adjacent_argument_save_groups(self):
        source = (
            "sw\t$20,32($sp)\n"
            "addu\t$20,$7,$zero\n"
            "sw\t$21,36($sp)\n"
            "addu\t$21,$6,$zero\n"
        )
        expected = (
            "sw\t$21,36($sp)\n"
            "addu\t$21,$6,$zero\n"
            "sw\t$20,32($sp)\n"
            "addu\t$20,$7,$zero\n"
        )
        self.assertEqual(reorder_aspsx_argument_save_groups(source), expected)

    def test_rejects_ordered_argument_save_groups(self):
        source = "sw\t$20,32($sp)\naddu\t$20,$6,$zero\n"
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            reorder_aspsx_argument_save_groups(source)

    def test_moves_terminal_stack_pop_into_return_delay_slot(self):
        source = (
            "lw\t$16,16($sp)\n"
            "addu\t$sp,$sp,24\n"
            "j\t$31\n"
            "nop  # DEBUG: branch/jump\n"
            ".end\tfunction\n"
        )
        expected = (
            "lw\t$16,16($sp)\n"
            "j\t$31\n"
            "addu\t$sp,$sp,24\n"
            ".end\tfunction\n"
        )
        self.assertEqual(reorder_aspsx_return_delay_slot(source), expected)

    def test_rejects_unrecognized_sequences(self):
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            reorder_aspsx_return_delay_slot("j\t$31\nnop\n")

    def test_moves_loop_label_after_inserted_load_delay(self):
        source = (
            "$L2:\n"
            "nop # DEBUG: Reuse of '$3'. 'lhu $2,4($3)' does not use $at\n"
            "# $L2:  # DEBUG: skipped\n"
            "lhu\t$2,4($3)\n"
        )
        expected = (
            "nop # DEBUG: Reuse of '$3'. 'lhu $2,4($3)' does not use $at\n"
            "$L2:\n"
            "lhu\t$2,4($3)\n"
        )
        self.assertEqual(move_aspsx_load_delay_label(source), expected)

    def test_rejects_mismatched_load_delay_labels(self):
        source = "$L2:\nnop # DEBUG: Reuse of '$3'\n# $L3: # DEBUG: skipped\n"
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            move_aspsx_load_delay_label(source)

    def test_moves_terminal_store_into_return_delay_slot(self):
        source = "sw\t$2,%lo(value)($at)\nj\t$31\nnop  # DEBUG: branch/jump\n"
        expected = "j\t$31\nsw\t$2,%lo(value)($at)\n"
        self.assertEqual(reorder_aspsx_jump_store_delay_slot(source), expected)

    def test_expands_symbolic_terminal_store_around_return(self):
        source = "sw\t$2,value\nj\t$31\nnop  # DEBUG: branch/jump\n"
        expected = (
            ".set\tnoat\n"
            "lui\t$at,%hi(value)\n"
            "j\t$31\n"
            "sw\t$2,%lo(value)($at)\n"
            ".set\tat\n"
        )
        self.assertEqual(reorder_aspsx_jump_store_delay_slot(source), expected)

    def test_expands_symbolic_store_around_call(self):
        source = "sw\t$4,value\njal\tcallee\nnop  # DEBUG: branch/jump\n"
        expected = (
            ".set\tnoat\n"
            "lui\t$at,%hi(value)\n"
            "jal\tcallee\n"
            "sw\t$4,%lo(value)($at)\n"
            ".set\tat\n"
        )
        self.assertEqual(reorder_aspsx_jump_store_delay_slot(source), expected)

    def test_rejects_nonstore_return_fill(self):
        with self.assertRaisesRegex(ValueError, "pattern not found"):
            reorder_aspsx_jump_store_delay_slot("addu\t$2,$3,$zero\nj\t$31\nnop\n")


if __name__ == "__main__":
    unittest.main()
