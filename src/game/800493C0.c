#include "game/types.h"

extern u16 D_8009AAE0[];

void func_800493C0(u32 index, u8 blue, u8 green, u8 red)
{
    if (index < 0x14) {
        D_8009AAE0[index] = (blue << 10) | (green << 5) | red;
    }
}
