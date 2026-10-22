#include "game/types.h"

extern s32 D_8009F360;

extern void func_800488A4(s32 index, u8 blue, u8 green, u8 red);

void func_80048978(s32 blue, s32 green, s32 red)
{
    if (D_8009F360 < 0x14) {
        func_800488A4(D_8009F360, blue, green, red);
        D_8009F360++;
    }
}
