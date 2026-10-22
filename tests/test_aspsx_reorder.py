import unittest

from tools.verify_matching_units import (
    fill_aspsx_symbolic_load_delay_slot,
    hoist_aspsx_small_data_load_before_prologue,
    move_aspsx_load_delay_label,
    reorder_aspsx_argument_save_groups,
    reorder_aspsx_vector_copy_call_setup,
    reuse_aspsx_symbolic_load_destination,
    reorder_aspsx_return_delay_slot,
    reorder_aspsx_jump_store_delay_slot,
)


class AspsxReorderTests(unittest.TestCase):
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
