#include "game/types.h"

s32 func_80015AF4(s32 origin, s32 first, s32 second, s32 *out)
{
    if (((s16)first - (s16)origin) * ((second >> 16) - (origin >> 16)) -
        ((s16)second - (s16)origin) * ((first >> 16) - (origin >> 16)) < 0) {
        return 1;
    }
    out[0] = origin;
    out[1] = first;
    out[2] = second;
    return 0;
}
