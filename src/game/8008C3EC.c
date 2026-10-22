#include "../../include/game/types.h"

extern u8 D_800A3C20[];

u8 *func_8008C3EC(u32 flags)
{
    if ((flags & 0xF0) != 0) {
        return D_800A3C20 + 0xF0;
    }
    return D_800A3C20;
}
