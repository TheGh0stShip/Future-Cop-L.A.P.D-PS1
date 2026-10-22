#include "game/types.h"

extern u8 D_8009FA20;
extern s32 D_8009F99C;

s32 func_8001F0FC(s32 callback)
{
    if (D_8009FA20 == 0) {
        return 3;
    }
    if (D_8009F99C != 0) {
        return 3;
    }
    D_8009F99C = callback;
    return 0;
}
