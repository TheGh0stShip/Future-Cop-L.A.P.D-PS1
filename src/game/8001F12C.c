#include "game/types.h"

extern s32 D_8009F99C;

s32 func_8001F12C(s32 callback)
{
    if (D_8009F99C == 0) {
        return 3;
    }
    if (callback == D_8009F99C) {
        D_8009F99C = 0;
        return 0;
    }
    return 3;
}
