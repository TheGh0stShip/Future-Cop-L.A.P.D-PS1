#include "game/types.h"

s32 func_8003B65C(s32 value, s32 range)
{
    if (value <= range >> 2) {
        return 0;
    }
    if (value < range >> 1) {
        return 1;
    }
    if (value < (range * 3) >> 2) {
        return 2;
    }
    return 3;
}
