#include "game/types.h"

extern s32 func_800963F0(void);
extern u32 func_80096484(void);

s32 func_800960CC(s32 mode)
{
    s32 result;

    if (mode == 0) {
        result = func_800963F0();
    } else {
        result = (func_80096484() >> 24) & 1;
    }

    return result;
}
