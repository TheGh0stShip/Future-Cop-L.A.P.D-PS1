#include "../../include/game/types.h"

extern s32 D_8009DD90;

s32 func_80091F60(s32 value)
{
    s32 previous;

    previous = D_8009DD90;
    D_8009DD90 = value;
    return previous;
}
