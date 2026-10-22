#include "game/types.h"

u32 func_80060DD8(u32 state)
{
    u32 mask20000;
    u32 mask10000;
    s32 i;

    mask20000 = 0x20000;
    mask10000 = 0x10000;
    for (i = 9; i >= 0; i--) {
        state <<= 1;
        state ^= (state & 0x4000) >> 3;
        state ^= (state & 0x800) >> 5;
        state ^= (state & mask20000) >> 2;
        state ^= (state & mask10000) >> 16;
    }
    return state;
}
