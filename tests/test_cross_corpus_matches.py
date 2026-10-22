import struct
import unittest

from tools.cross_corpus_matches import normalize_jump_targets, scan


class CrossCorpusMatchTests(unittest.TestCase):
    def test_normalization_masks_only_jump_destination(self):
        words = struct.pack("<III", 0x0C123456, 0x08123456, 0x24020001)
        normalized = normalize_jump_targets(words)
        self.assertEqual(
            struct.unpack("<III", normalized),
            (0x0C000000, 0x08000000, 0x24020001),
        )

    def test_scan_separates_exact_and_jump_normalized_hits(self):
        target = struct.pack("<III", 0x24020001, 0x0C123456, 0x03E00008)
        corpus = target + struct.pack(
            "<III", 0x24020001, 0x0C654321, 0x03E00008
        )
        functions = [{
            "name": "sample", "addr": 0x80010000, "size": 12,
            "is_data": False, "alias_of": 0,
        }]
        report = scan(
            target, functions, corpus, 0x80010000, 0x80020000, 8, 32
        )
        self.assertEqual(len(report["exact_hits"]), 1)
        self.assertEqual(len(report["jump_normalized_hits"]), 1)
        self.assertEqual(
            report["jump_normalized_hits"][0]["corpus_address"], 0x8002000C
        )


if __name__ == "__main__":
    unittest.main()
