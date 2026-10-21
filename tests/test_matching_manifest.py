import json
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class MatchingManifestTests(unittest.TestCase):
    def test_units_are_unique_nonoverlapping_and_have_sources(self):
        manifest = json.loads((ROOT / "config/us/matching_units.json").read_text())
        ranges = []
        for unit in manifest["units"]:
            address = int(unit["address"], 0)
            size = int(unit["size"], 0)
            self.assertGreater(size, 0)
            self.assertTrue((ROOT / unit["source"]).is_file())
            ranges.append((address, address + size, unit["name"]))
        ranges.sort()
        self.assertEqual(len({item[2] for item in ranges}), len(ranges))
        for left, right in zip(ranges, ranges[1:]):
            self.assertLessEqual(left[1], right[0])


if __name__ == "__main__":
    unittest.main()
