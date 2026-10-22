import unittest

from tools.generate_decomp_report import build_report


class DecompReportTests(unittest.TestCase):
    def test_report_tracks_only_manifested_exact_matches(self):
        report = build_report()
        measures = report["measures"]
        self.assertEqual(report["version"], 2)
        self.assertEqual(measures["matched_code"], "24208")
        self.assertEqual(measures["total_code"], "555552")
        self.assertEqual(measures["matched_functions"], 311)
        self.assertEqual(measures["total_functions"], 1869)
        self.assertEqual(measures["total_data"], "305279768")
        self.assertEqual(measures["matched_data"], "27648404")
        self.assertEqual(
            sum(unit["metadata"]["complete"] for unit in report["units"]), 311
        )
        categories = {item["id"]: item["measures"] for item in report["categories"]}
        self.assertEqual(categories["executable_data"]["total_data"], "32224")
        self.assertEqual(categories["executable_data"]["matched_data"], "404")
        self.assertEqual(categories["disc_assets"]["total_data"], "305247544")
        self.assertEqual(categories["disc_assets"]["matched_data"], "27648000")


if __name__ == "__main__":
    unittest.main()
