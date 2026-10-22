#include "../../include/game/types.h"

u32 func_80093F58(int textured, int dither, u32 mode)
{
    u32 command = 0xE1000000;
    u32 value = mode & 0x9FF;

    if (textured != 0) {
        command |= 0x200;
    }
    if (dither != 0) {
        value |= 0x400;
    }
    return command | value;
}
