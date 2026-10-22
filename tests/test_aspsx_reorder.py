import unittest

from tools.verify_matching_units import (
    move_aspsx_load_delay_label,
    reorder_aspsx_return_delay_slot,
)


class AspsxReorderTests(unittest.TestCase):
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


if __name__ == "__main__":
    unittest.main()
