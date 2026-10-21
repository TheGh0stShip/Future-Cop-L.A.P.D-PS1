import json
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]


class FunctionInventoryTests(unittest.TestCase):
    def test_inventory_is_ordered_and_in_range(self):
        report = json.loads((ROOT / "build/us/function_candidates.json").read_text())
        addresses = report["addresses"]
        self.assertEqual(addresses, sorted(set(addresses)))
        self.assertGreaterEqual(len(addresses), 1400)
        self.assertTrue(all(0x80010000 <= value < 0x8009F800 and value % 4 == 0
                            for value in addresses))

    def test_entrypoint_is_in_inventory(self):
        report = json.loads((ROOT / "build/us/function_candidates.json").read_text())
        self.assertIn(0x80096520, report["addresses"])


if __name__ == "__main__":
    unittest.main()
