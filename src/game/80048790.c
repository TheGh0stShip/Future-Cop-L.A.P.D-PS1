#include "game/types.h"

extern s32 D_8009FAB8;
extern s32 D_8009F760;

u8 func_80048790(void)
{
    if (D_8009FAB8 == 0) {
        return 0;
    }
    if (D_8009FAB8 != 2) {
        return 0;
    }
    if (D_8009F760 != 0) {
        return 0;
    }
    return 1;
}
