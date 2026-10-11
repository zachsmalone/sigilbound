import unittest
import build_rom

class BuildTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls): cls.rom=build_rom.make_rom()
    def test_size(self): self.assertEqual(len(self.rom),32768)
    def test_logo(self): self.assertEqual(self.rom[0x104:0x134],build_rom.LOGO)
    def test_header(self): self.assertEqual((sum(self.rom[0x134:0x14E])+25)&255,0)
    def test_global(self):
        self.assertEqual(int.from_bytes(self.rom[0x14E:0x150],"big"),(sum(self.rom[:0x14E])+sum(self.rom[0x150:]))&65535)
    def test_deterministic(self): self.assertEqual(self.rom,build_rom.make_rom())
    def test_party_fixture(self):
        self.assertEqual(self.rom[0x2000:0x2006],b"SGBP\x01\x01")
        self.assertEqual(self.rom[0x2010:0x2014],bytes([1,0,1,5]))
        self.assertEqual(self.rom[0x2018:0x201A],bytes([24,0]))

if __name__=="__main__":unittest.main()
