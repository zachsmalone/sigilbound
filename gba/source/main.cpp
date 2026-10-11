#include <cstdint>
#include "battle.hpp"
using u16 = std::uint16_t;
static volatile u16& display = *reinterpret_cast<volatile u16*>(0x04000000);
static volatile u16& scanline = *reinterpret_cast<volatile u16*>(0x04000006);
static volatile u16& keys = *reinterpret_cast<volatile u16*>(0x04000130);
static volatile u16* const pixels = reinterpret_cast<volatile u16*>(0x06000000);
constexpr u16 rgb(int r, int g, int b) { return r | (g<<5) | (b<<10); }
void rect(int x, int y, int w, int h, u16 color) {
    for (int yy=y; yy<y+h; ++yy) for (int xx=x; xx<x+w; ++xx)
        if (xx>=0 && xx<240 && yy>=0 && yy<160) pixels[yy*240+xx]=color;
}
unsigned glyph(char ch) {
    switch (ch) {
    case 'A': return 0x2BED;
    case 'B': return 0x6BAE;
    case 'C': return 0x3923;
    case 'D': return 0x6B6E;
    case 'E': return 0x79A7;
    case 'F': return 0x79A4;
    case 'G': return 0x396B;
    case 'H': return 0x5BED;
    case 'I': return 0x7497;
    case 'J': return 0x126A;
    case 'K': return 0x5BAD;
    case 'L': return 0x4927;
    case 'M': return 0x5FED;
    case 'N': return 0x5FFD;
    case 'O': return 0x2B6A;
    case 'P': return 0x6BA4;
    case 'Q': return 0x2B7B;
    case 'R': return 0x6BAD;
    case 'S': return 0x388E;
    case 'T': return 0x7492;
    case 'U': return 0x5B6F;
    case 'V': return 0x5B6A;
    case 'W': return 0x5BFD;
    case 'X': return 0x5AAD;
    case 'Y': return 0x5A92;
    case 'Z': return 0x72A7;
    case '0': return 0x7B6F;
    case '1': return 0x2C97;
    case '2': return 0x62A7;
    case '3': return 0x628E;
    case '4': return 0x5BC9;
    case '5': return 0x798E;
    case '6': return 0x39EF;
    case '7': return 0x7292;
    case '8': return 0x7BEF;
    case '9': return 0x7BCE;
    default: return 0;
    }
}
void text(int x, int y, const char* value) {
    for (; *value; ++value, x+=8) {
        const unsigned bits=glyph(*value);
        for (int row=0; row<5; ++row) for (int col=0; col<3; ++col)
            if (bits & (1u << (14-row*3-col))) rect(x+col*2,y+row*2,2,2,rgb(29,29,27));
    }
}
void number(int x, int y, int value) {
    char digits[3]={static_cast<char>('0'+value/10),static_cast<char>('0'+value%10),0};
    text(x,y,digits);
}
static const char* const wolf[16]={
    "0033000033000000","0032300323000000","0032233223000000","0003222223300000",
    "0003222231233000","0003222222223300","0000322222111330","0000322221113300",
    "0033322223333000","0322222222230000","3222222222223000","3222222222223000",
    "0333222222223000","0003223332230000","0003230032300000","0003330033300000"};
void init_sprites() {
    auto palette=reinterpret_cast<volatile u16*>(0x05000200);
    palette[1]=rgb(28,26,19); palette[2]=rgb(15,17,22); palette[3]=rgb(3,4,7);
    // Mode 3 OBJ tiles: 0x06014000–0x06017FFF, OAM tile indices 512–1023.
    auto tiles=reinterpret_cast<volatile u16*>(0x06014000);
    for (int tile=0; tile<4; ++tile) for (int row=0; row<8; ++row)
        for (int half=0; half<2; ++half) {
            u16 packed=0;
            for (int col=0; col<4; ++col)
                packed |= (wolf[(tile/2)*8+row][(tile%2)*8+half*4+col]-'0') << (col*4);
            tiles[tile*16+row*2+half]=packed;
        }
    auto oam=reinterpret_cast<volatile u16*>(0x07000000);
    for (int i=0; i<128; ++i) { oam[i*4]=0x0200; oam[i*4+1]=0; oam[i*4+2]=0; oam[i*4+3]=0; }
    oam[0]=58; oam[1]=40 | (1<<14); oam[2]=512;
    oam[4]=58; oam[5]=184 | (1<<14) | (1<<12); oam[6]=512;
}
void draw(const sigilbound::Battle& b) {
    display=0x0080; // Forced blank while updating framebuffer and OBJ data.
    rect(0,0,240,160,rgb(3,5,9));
    rect(10,47,220,34,rgb(7,10,13));
    text(12,8,"SIGILBOUND GBA"); text(20,30,"NUGWOLF"); text(136,30,"WILD NUGWOLF");
    text(20,88,"HP"); number(48,88,b.hp); text(144,88,"HP"); number(172,88,b.enemy_hp);
    rect(20,104,80,5,rgb(10,7,8)); rect(20,104,b.hp*80/24,5,rgb(8,27,13));
    rect(140,104,80,5,rgb(10,7,8)); rect(140,104,b.enemy_hp*80/18,5,rgb(27,15,8));
    const char* status=b.result==sigilbound::Result::victory ? "VICTORY" :
        b.result==sigilbound::Result::defeat ? "DEFEAT" : "BATTLE";
    text(12,116,status); text(112,116,"XP"); number(136,116,b.xp);
    text(12,132,"USES A"); number(64,132,b.tackle); text(112,132,"B"); number(128,132,b.brace);
    text(12,148,"A ATTACK B BRACE START RESET");
    init_sprites(); display=0x1443; // Mode 3, BG2, OBJ, 1D OBJ mapping.
}
int main() {
    sigilbound::Battle battle;
    draw(battle);
    unsigned previous=0;
    for (;;) {
        while (scanline>=160) {}
        while (scanline<160) {}
        const unsigned held=(~keys)&0x03FF;
        const unsigned pressed=held & ~previous;
        previous=held;
        bool changed=false;
        if (pressed & 8) { battle.reset(); changed=true; }
        else if (pressed & 1) changed=battle.act(false);
        else if (pressed & 2) changed=battle.act(true);
        if (changed) draw(battle);
    }
}
