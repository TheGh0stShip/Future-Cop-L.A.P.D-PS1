#include "game/types.h"

extern volatile s32 D_80010000;

s32 func_8001C478(void)
{
    while (D_80010000 != 0) {
    }
    return 0;
}
