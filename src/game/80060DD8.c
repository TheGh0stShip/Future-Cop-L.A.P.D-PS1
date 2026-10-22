#include "game/types.h"

u32 func_80060DD8(u32 state)
{
    s32 i;

    for (i = 9; i >= 0; i--) {
        state <<= 1;
        state ^= (state & 0x4000) >> 3;
        state ^= (state & 0x800) >> 5;
        state ^= (state & 0x20000) >> 2;
        state ^= (state & 0x10000) >> 16;
    }
    return state;
}
