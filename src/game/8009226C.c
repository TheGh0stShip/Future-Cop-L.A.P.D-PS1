#include "game/types.h"

u16 func_8009226C(s32 x, s32 y)
{
    return (y << 6) | ((x >> 4) & 0x3F);
}
