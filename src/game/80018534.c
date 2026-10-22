#include "game/types.h"

extern u16 D_8009A234[];

s32 func_80018534(u32 angle)
{
    s32 index;
    s32 value;

    angle &= 0xFFF;
    index = angle & 0x3FF;
    if (angle & 0x400) {
        index = 0x400 - index;
    }
    value = D_8009A234[index] << 1;
    if (angle & 0x800) {
        value = -value;
    }
    return value;
}
