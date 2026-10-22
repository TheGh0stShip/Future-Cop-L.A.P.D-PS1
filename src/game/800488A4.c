#include "game/types.h"

extern u16 D_800A1E10[][0x10];
extern s32 D_8009F360;

void func_800488A4(s32 index, u8 blue, u8 green, u8 red)
{
    u16 *color;
    s32 i;
    s32 b;
    s32 g;
    s32 r;
    u16 mask;

    if (index < 0 || D_8009F360 < index) {
        return;
    }
    D_800A1E10[index][0] = 0;
    color = &D_800A1E10[index][1];
    r = red * 2;
    g = green * 2;
    b = blue * 2;
    for (i = 2; i < 0x11; i++) {
        mask = 0;
        if (i < 8) {
            mask = 0x8000;
        }
        *color++ = mask | ((b / 16) << 10 | (g / 16) << 5 | r / 16);
        r += red;
        g += green;
        b += blue;
    }
}
