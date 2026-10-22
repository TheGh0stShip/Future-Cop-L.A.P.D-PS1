#include "game/types.h"

extern s16 D_8009F580;

void func_80013AA0(s32 low, s32 high)
{
    D_8009F580 = (high << 8) | low;
}
