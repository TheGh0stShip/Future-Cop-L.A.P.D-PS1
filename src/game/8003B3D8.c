#include "../../include/game/types.h"

extern u32 D_800A6578[];

u32 *func_8003B3D8(u32 column, u32 row)
{
    return &D_800A6578[(column & 0xFF) + row * 0x100];
}
