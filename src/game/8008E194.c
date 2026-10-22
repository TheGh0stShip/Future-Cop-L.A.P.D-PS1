#include "../../include/game/types.h"

extern s32 D_8009C8A8;

s32 func_8008E194(s32 value)
{
    s32 previous;

    previous = D_8009C8A8;
    D_8009C8A8 = value;
    return previous;
}
