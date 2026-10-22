import unittest

from tools.scan_psyq_signatures import longest_anchor, masked_matches, parse_signature


class PsyqSignatureTests(unittest.TestCase):
    def test_parse_signature_tracks_wildcards(self):
        values, masks = parse_signature("12 ?? aB 00")
        self.assertEqual(values, bytes([0x12, 0, 0xAB, 0]))
        self.assertEqual(masks, bytes([0xFF, 0, 0xFF, 0xFF]))

    def test_longest_anchor_prefers_first_longest_run(self):
        values, masks = parse_signature("01 02 ?? 03 04 05 ?? 06")
        self.assertEqual(longest_anchor(values, masks), (3, bytes([3, 4, 5])))

    def test_masked_match_verifies_complete_pattern(self):
        values, masks = parse_signature("11 22 33 44 ?? 66 77 88 99")
        payload = bytes.fromhex("00 11 22 33 44 55 66 77 88 99 11 22 33 44")
        self.assertEqual(masked_matches(payload, values, masks), [1])

    def test_short_anchor_is_rejected(self):
        values, masks = parse_signature("11 22 ?? 33 44")
        self.assertEqual(masked_matches(bytes.fromhex("11 22 00 33 44"), values, masks), [])


if __name__ == "__main__":
    unittest.main()
