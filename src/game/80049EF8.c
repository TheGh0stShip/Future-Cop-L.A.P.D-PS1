#include "game/types.h"

s32 func_80049EF8(s32 origin, s32 first, s32 second)
{
    return ((s16)first - (s16)origin) * ((second >> 16) - (origin >> 16)) -
           ((s16)second - (s16)origin) * ((first >> 16) - (origin >> 16)) >= 0;
}
