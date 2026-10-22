#include "../../include/game/types.h"

extern u32 D_800A6578[];

u32 *func_8003B3D8(u32 column, u32 row)
{
    u32 rowOffset;
    u32 columnOffset;

    rowOffset = row << 10;
    columnOffset = (column & 0xFF) << 2;
    return (u32 *)((u8 *)D_800A6578 + rowOffset + columnOffset);
}
