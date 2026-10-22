#include "game/types.h"

extern u8 D_8009F234;

extern s32 func_8001D248(s32 arg0, s32 arg1, s32 arg2);

s32 func_8001CE10(void)
{
    if (D_8009F234 != 1 && func_8001D248(0, 0, 0) < 0) {
        return -2;
    }
    return 0;
}
