import json
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

from tools.sweep_candidate_compilers import compare, score, sweep


class CompilerSweepTests(unittest.TestCase):
    def test_exact_output_allows_only_zero_alignment_tail(self):
        result = compare(b"\x01\x02\x03\x04", b"\x01\x02\x03\x04\x00\x00")
        self.assertTrue(result["exact"])
        self.assertEqual(result["matching_words"], 1)
        self.assertEqual(result["nonzero_tail_bytes"], 0)

    def test_mismatch_records_containing_words(self):
        result = compare(b"\x01\x02\x03\x04", b"\x01\x09\x03\x04")
        self.assertFalse(result["exact"])
        self.assertEqual(result["first_mismatch"], 1)
        self.assertEqual(result["mismatch_word_offset"], 0)
        self.assertEqual(result["expected_word"], "01020304")
        self.assertEqual(result["actual_word"], "01090304")

    def test_score_prefers_more_matching_words(self):
        better = compare(b"abcdefgh", b"abcdxfgh")
        worse = compare(b"abcdefgh", b"abxxxxxx")
        self.assertGreater(score(better), score(worse))

    def test_sweep_can_select_one_named_unit(self):
        config = {
            "compiler": "unused",
            "compiler_flags": [],
            "maspsx_flags": [],
            "units": [
                {"name": "first", "address": "0x80010000", "size": "0x4",
                 "source": "first.c", "symbols": {}},
                {"name": "second", "address": "0x80010004", "size": "0x4",
                 "source": "second.c", "symbols": {}},
            ],
        }
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            config_path = root / "units.json"
            executable_path = root / "game.exe"
            config_path.write_text(json.dumps(config))
            executable_path.write_bytes(bytes(0x808))
            with patch(
                "tools.sweep_candidate_compilers.verifier.validate"
            ), patch(
                "tools.sweep_candidate_compilers.verifier.compile_unit",
                return_value=bytes(4),
            ) as compile_unit:
                report = sweep(
                    config_path, executable_path, ["test-compiler"], "unused-",
                    {"second"},
                )
        self.assertEqual(report["candidate_functions"], 1)
        self.assertEqual(report["units"][0]["name"], "second")
        self.assertTrue(compile_unit.call_args.args[0]["link"])


if __name__ == "__main__":
    unittest.main()
