#include "../../include/game/types.h"

extern s32 D_8009C8AC;

s32 func_8008E1A8(s32 value)
{
    s32 previous;

    previous = D_8009C8AC;
    D_8009C8AC = value;
    return previous;
}
