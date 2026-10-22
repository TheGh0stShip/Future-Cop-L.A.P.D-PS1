#include "game/types.h"

struct Unk1C040 {
    u8 unk00[0x18];
    u8 enabled;
    u8 red;
    u8 green;
    u8 blue;
    u8 unk1C[0x40];
};

extern u8 D_8009F225;
extern struct Unk1C040 D_800A06E0[];

void func_8001C040(u32 color)
{
    s32 i;

    for (i = 0; i < D_8009F225; i++) {
        D_800A06E0[i].red = color >> 16;
        D_800A06E0[i].green = color >> 8;
        D_800A06E0[i].blue = color;
    }
}
