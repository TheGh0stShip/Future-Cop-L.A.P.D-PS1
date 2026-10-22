#include "game/types.h"

struct DrawTPage {
    u32 addr : 24;
    u32 len : 8;
    u32 code;
};

extern u8 *D_8009FA90[];
extern u8 *D_8009F9C0[];
extern u16 D_800A1940[];
extern u16 D_8009F768;
extern s32 D_8009F76C;

extern s32 func_80048F98(s32 character, s32 x, s32 y, u16 color);

void func_80049100(u8 *text, s32 x, s32 y, s32 color)
{
    struct DrawTPage *packet;
    u32 character;

    if (D_8009F768 == 0) {
        return;
    }
    packet = (struct DrawTPage *)D_8009FA90[0];
    if (D_8009FA90[0] < D_8009F9C0[0]) {
        D_8009FA90[0] = (u8 *)packet + 0x1C;
    }
    packet->addr = 0;
    packet->len = 1;
    packet->code = (D_800A1940[D_8009F76C] & 0x9FF) | 0xE1000000;
    while ((character = *text) != 0) {
        text++;
        if (character - 10 < 0x16) {
            color = character - 10;
        } else {
            x += func_80048F98(character, x, y, color);
        }
    }
}
