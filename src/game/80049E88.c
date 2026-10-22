#include "game/types.h"

extern s32 func_80049C38(u8 *data, s32 arg1);

s32 func_80049E88(u8 *data, s32 arg1)
{
    return func_80049C38(data, arg1) != 0;
}
