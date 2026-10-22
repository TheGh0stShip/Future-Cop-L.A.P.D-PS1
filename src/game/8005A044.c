#include "../../include/game/types.h"

extern u8 D_8009FA8C[];

void func_8005A044(void)
{
    s32 index;

    for (index = 1; index >= 0; index--) {
        D_8009FA8C[index] = 0;
    }
}
