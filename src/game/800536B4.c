#include "game/types.h"

extern s32 D_8009FA64;

void func_800536B4(s32 *near, s32 *far)
{
    s32 distance;

    distance = D_8009FA64;
    *near = (distance << 8) / 608;
    if (distance >= 0x1A000) {
        *far = ((distance - 0x1A000) << 8) / 192;
    } else {
        *far = 0;
    }
}
