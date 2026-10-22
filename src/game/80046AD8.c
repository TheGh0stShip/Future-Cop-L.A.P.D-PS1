#include "game/types.h"

extern s32 func_80046B04(s32 arg0, s32 arg1, s32 arg2, s32 slot);

s32 func_80046AD8(void)
{
    return func_80046B04(0x7F, 0xA, 2, 0xFF);
}
