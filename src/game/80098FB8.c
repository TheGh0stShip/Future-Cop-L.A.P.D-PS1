#include "../../include/game/types.h"

extern s32 D_8009EEDC;

void func_80098FB8(s32 value)
{
    if (value != D_8009EEDC) {
        D_8009EEDC = value;
    }
}
