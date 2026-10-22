import unittest

from tools.generate_decomp_report import build_report


class DecompReportTests(unittest.TestCase):
    def test_report_tracks_only_manifested_exact_matches(self):
        report = build_report()
        measures = report["measures"]
        self.assertEqual(report["version"], 2)
        self.assertEqual(measures["matched_code"], "1708")
        self.assertEqual(measures["total_code"], "555552")
        self.assertEqual(measures["matched_functions"], 69)
        self.assertEqual(measures["total_functions"], 1886)
        self.assertEqual(
            sum(unit["metadata"]["complete"] for unit in report["units"]), 69
        )


if __name__ == "__main__":
    unittest.main()
