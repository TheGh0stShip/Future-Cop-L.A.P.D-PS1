import importlib.util
import json
import sys
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
SPEC = importlib.util.spec_from_file_location(
    "local_proposer", ROOT / "tools/local_proposer.py"
)
proposer = importlib.util.module_from_spec(SPEC)
sys.modules[SPEC.name] = proposer
SPEC.loader.exec_module(proposer)


class LocalProposerTests(unittest.TestCase):
    def test_unwrap_plain_and_fenced_json(self):
        expected = {"source": "int f(void) { return 0; }", "symbols": {}}
        self.assertEqual(json.loads(proposer.unwrap_json(json.dumps(expected))), expected)
        fenced = "```json\n" + json.dumps(expected) + "\n```"
        self.assertEqual(json.loads(proposer.unwrap_json(fenced)), expected)

    def test_prompt_contains_complete_request(self):
        request = {"protocol": 1, "target": {"name": "func_80010000"}}
        text = proposer.prompt(request)
        self.assertIn("func_80010000", text)
        self.assertIn('"protocol": 1', text)


if __name__ == "__main__":
    unittest.main()
