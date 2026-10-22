#include "game/types.h"

extern u32 *func_8003B3D8(u32 column, u32 row);

void func_8003C4C4(u8 column, s32 row, u8 bits)
{
    *(u8 *)func_8003B3D8(column, row) |= bits;
}
