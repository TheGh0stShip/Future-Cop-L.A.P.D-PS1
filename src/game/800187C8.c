#include "game/types.h"

u32 func_800187C8(u32 value)
{
    u32 root;
    u32 remainder;
    u32 trial;
    s32 i;

    root = 0;
    remainder = root;
    i = 0x17;
    do {
        remainder = (remainder << 2) | (value >> 30);
        root <<= 1;
        trial = (root << 1) + 1;
        value <<= 2;
        if (remainder >= trial) {
            remainder -= trial;
            root++;
        }
    } while (--i != -1);
    return root;
}
