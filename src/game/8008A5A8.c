#include "../../include/game/types.h"

extern s32 D_8009C790;

s32 func_8008A5A8(void)
{
    s32 previous;

    previous = D_8009C790;
    D_8009C790 = 0;
    return previous;
}
