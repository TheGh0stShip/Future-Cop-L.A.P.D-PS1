import unittest

from tools.sweep_candidate_compilers import compare, score


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


if __name__ == "__main__":
    unittest.main()
