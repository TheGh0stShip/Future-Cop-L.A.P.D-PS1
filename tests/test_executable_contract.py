import hashlib
import struct
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
EXE = ROOT / "baseroms/us/SLUS_007.39"


class ExecutableContractTests(unittest.TestCase):
    def test_supported_executable(self):
        data = EXE.read_bytes()
        self.assertEqual(len(data), 0x90000)
        self.assertEqual(data[:8], b"PS-X EXE")
        self.assertEqual(hashlib.sha256(data).hexdigest(),
                         "892f8482d3c9b163f45afae50676808e5673bdb7d6c079663cc1fdf6826b236f")
        self.assertEqual(struct.unpack_from("<I", data, 0x10)[0], 0x80096520)
        self.assertEqual(struct.unpack_from("<I", data, 0x18)[0], 0x80010000)
        self.assertEqual(struct.unpack_from("<I", data, 0x1C)[0], 0x8F800)


if __name__ == "__main__":
    unittest.main()
