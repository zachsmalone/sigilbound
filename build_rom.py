from pathlib import Path
import hashlib, json
from PIL import Image, ImageDraw, ImageFont

LOGO = bytes([206,237,102,102,204,13,0,11,3,115,0,131,0,12,0,13,0,8,17,31,136,137,0,14,220,204,110,230,221,221,217,153,187,187,103,99,110,14,236,204,221,220,153,159,187,185,51,62])
HP, TACKLE, BRACE, XP = 0xC118, 0xC12D, 0xC12E, 0xC114

class Emitter:
    def __init__(self, rom):
        self.rom, self.pc, self.labels, self.fixups = rom, 0x150, {}, []
    def emit(self, *values):
        self.rom[self.pc:self.pc+len(values)] = bytes(values)
        self.pc += len(values)
    def label(self, name): self.labels[name] = self.pc
    def jump(self, opcode, name):
        self.emit(opcode, 0, 0)
        self.fixups.append((self.pc-2, name))
    def word(self, value): self.emit(value & 255, value >> 8)
    def hl(self, value): self.emit(0x21); self.word(value)
    def de(self, value): self.emit(0x11); self.word(value)
    def bc(self, value): self.emit(0x01); self.word(value)
    def load(self, address): self.emit(0xFA); self.word(address)
    def store(self, address): self.emit(0xEA); self.word(address)
    def resolve(self):
        for position, name in self.fixups:
            self.rom[position:position+2] = self.labels[name].to_bytes(2, "little")

def font_tiles():
    font = ImageFont.load_default(size=8)
    result = bytearray()
    for code in range(128):
        image = Image.new("1", (8,8))
        if 32 <= code < 127:
            ImageDraw.Draw(image).text((0,-2), chr(code), font=font, fill=1)
        for y in range(8):
            bits = sum(1 << (7-x) for x in range(8) if image.getpixel((x,y)))
            result.extend((bits,bits))
    return bytes(result)

def make_rom():
    rom = bytearray(32768)
    rom[0x100:0x104] = bytes([0,0xC3,0x50,1])
    rom[0x104:0x134] = LOGO
    rom[0x134:0x143] = b"SIGIL BATTLE".ljust(15,b"\0")
    rom[0x14A] = 1
    a = Emitter(rom)
    e, lab, j = a.emit, a.label, a.jump
    hl, de, bc, load, store = a.hl, a.de, a.bc, a.load, a.store
    e(0xF3,0x31,0xFE,0xFF,0xAF); store(0xFFFF); e(0xE0,0x26)
    lab("boot_wait"); e(0xF0,0x44,0xFE,144); j(0xDA,"boot_wait")
    e(0xAF,0xE0,0x40,0xE0,0x42,0xE0,0x43)
    de(0x1000); hl(0x8000); bc(2048); j(0xCD,"copy")
    e(0x3E,0xE4,0xE0,0x47); j(0xCD,"init")
    lab("loop"); lab("visible"); e(0xF0,0x44,0xFE,144); j(0xD2,"visible")
    lab("vblank"); e(0xF0,0x44,0xFE,144); j(0xDA,"vblank")
    e(0x3E,0x10,0xE0,0,0xF0,0,0xF0,0,0x2F,0xE6,3,0x47,0xB7); j(0xCA,"released")
    load(0xC000); e(0xB7); j(0xC2,"loop"); e(0x3E,1); store(0xC000)
    load(0xC001); e(0xB7); j(0xC2,"restart")
    e(0x78,0xE6,1); j(0xC2,"attack"); j(0xC3,"brace")
    lab("released"); e(0xAF); store(0xC000); j(0xC3,"loop")
    lab("restart"); j(0xCD,"init"); e(0x3E,1); store(0xC000); j(0xC3,"loop")
    lab("attack"); load(TACKLE); e(0xB7); j(0xCA,"loop")
    e(0x3D); store(TACKLE); e(0xAF); store(0xC002)
    load(0xC300); e(0xFE,4); j(0xDA,"win"); e(0xD6,3); store(0xC300); j(0xC3,"enemy")
    lab("brace"); load(BRACE); e(0xB7); j(0xCA,"loop")
    e(0x3D); store(BRACE); e(0x3E,1); store(0xC002)
    lab("enemy"); load(0xC002); e(0xB7); j(0xC2,"guarded")
    load(HP); e(0xFE,3); j(0xDA,"lose"); e(0xD6,2); store(HP); j(0xC3,"redraw")
    lab("guarded"); load(HP); e(0xFE,2); j(0xDA,"lose"); e(0x3D); store(HP)
    lab("redraw"); j(0xCD,"draw"); j(0xC3,"loop")
    lab("win"); e(0xAF); store(0xC300); e(0x3E,1); store(0xC001)
    e(0x3E,20); store(XP); j(0xC3,"redraw")
    lab("lose"); e(0xAF); store(HP); e(0x3E,2); store(0xC001); j(0xC3,"redraw")
    lab("init"); hl(0xC100); bc(400)
    lab("party_clear"); e(0xAF,0x22,0x0B,0x78,0xB1); j(0xC2,"party_clear")
    de(0x2000); hl(0xC100); bc(80); j(0xCD,"copy")
    e(0xAF); store(0xC000); store(0xC001); store(0xC002)
    e(0x3E,18); store(0xC300); j(0xCD,"draw"); e(0xC9)
    lab("draw"); e(0xF0,0x40,0xE6,0x80); j(0xCA,"draw_ready")
    e(0xF0,0x44,0xFE,144); j(0xDA,"draw")
    lab("draw_ready"); e(0xAF,0xE0,0x40); hl(0x9800); bc(1024)
    lab("clear"); e(0xAF,0x22,0x0B,0x78,0xB1); j(0xC2,"clear")
    de(0x1800); hl(0x9800); bc(576); j(0xCD,"copy")
    for address,row,column in ((HP,3,12),(0xC300,6,12),(TACKLE,9,14),(BRACE,10,14),(XP,12,12)):
        load(address); hl(0x9800+row*32+column); j(0xCD,"decimal")
    load(0xC001); e(0xB7); j(0xCA,"lcd_on"); e(0xFE,1); j(0xCA,"victory_text")
    de(0x2120); j(0xC3,"result_text")
    lab("victory_text"); de(0x2100)
    lab("result_text"); hl(0x9800+14*32); bc(20); j(0xCD,"copy")
    lab("lcd_on"); e(0x3E,0x91,0xE0,0x40,0xC9)
    lab("decimal"); e(0x06,0)
    lab("tens"); e(0xFE,10); j(0xDA,"digits"); e(0xD6,10,0x04); j(0xC3,"tens")
    lab("digits"); e(0x4F,0x78,0xC6,48,0x22,0x79,0xC6,48,0x77,0xC9)
    lab("copy"); e(0x1A,0x22,0x13,0x0B,0x78,0xB1); j(0xC2,"copy"); e(0xC9)
    assert a.pc < 0x1000
    a.resolve()
    rom[0x1000:0x1800] = font_tiles()
    lines = ["SIGILBOUND BATTLE","Repository baseline","","Nugwolf HP: 00/24","","","Wild HP:    00/18","","A: Tackle 3 damage","Tackle uses:  00","Brace uses:   00","B: Brace halves hit","Nugwolf XP:  00","","Battle in progress","End: A/B restarts","No saves or bonding",""]
    assert len(lines)==18 and all(len(line)<=20 for line in lines)
    rom[0x1800:0x1A40] = "".join(line.ljust(32) for line in lines).encode("ascii")
    party = bytearray(80); party[:6] = b"SGBP\x01\x01"
    party[16:28] = bytes([1,0,1,5,0,0,0,0,24,0,0,0])
    party[40:49] = bytes([2,1,2,0,0,35,20,0,0])
    rom[0x2000:0x2050] = party
    rom[0x2100:0x2114] = b"VICTORY +20 XP".ljust(20)
    rom[0x2120:0x2134] = b"DEFEAT".ljust(20)
    rom[0x14D] = (-sum(rom[0x134:0x14D])-25)&255
    rom[0x14E:0x150] = ((sum(rom[:0x14E])+sum(rom[0x150:]))&65535).to_bytes(2,"big")
    assert len(rom)==32768
    return bytes(rom)

def build(folder="output"):
    rom = make_rom(); destination = Path(folder)
    destination.mkdir(parents=True,exist_ok=True)
    (destination/"Sigilbound_Battle_Baseline.gb").write_bytes(rom)
    report = {"bytes":len(rom),"sha256":hashlib.sha256(rom).hexdigest(),
              "full_emulator_tested":False,"delta_tested":False,
              "note":"Repository reconstruction with bundled Pillow font; not identical to earlier ROM."}
    (destination/"validation.json").write_text(json.dumps(report,indent=2))
    return report

if __name__=="__main__": print(json.dumps(build(),indent=2))
