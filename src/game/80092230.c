#include "game/types.h"

u16 func_80092230(s32 type, s32 blend, s32 x, s32 y)
{
    return ((type & 3) << 7)
        | ((blend & 3) << 5)
        | ((y & 0x100) >> 4)
        | ((x & 0x3FF) >> 6)
        | ((y & 0x200) << 2);
}
