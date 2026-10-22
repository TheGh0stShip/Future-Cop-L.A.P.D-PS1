#include "game/types.h"

extern s32 func_8009635C(void);
extern u32 func_80096484(void);

s32 func_80096090(s32 mode)
{
    s32 result;

    if (mode == 0) {
        result = func_8009635C();
    } else {
        result = (func_80096484() >> 29) & 1;
    }

    return result;
}
