#include "game/types.h"

s32 func_800195F0(u32 value)
{
    s32 count;

    count = 32;
    if ((value << 16) != 0) {
        value <<= 16;
        count = 16;
    }
    if ((value << 8) != 0) {
        value <<= 8;
        count -= 8;
    }
    if ((value << 4) != 0) {
        value <<= 4;
        count -= 4;
    }
    if ((value << 2) != 0) {
        value <<= 2;
        count -= 2;
    }
    if ((value << 1) != 0) {
        value <<= 1;
        count -= 1;
    }
    if (value != 0) {
        count -= 1;
    }
    return count;
}
