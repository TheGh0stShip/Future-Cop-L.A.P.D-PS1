#include "../../include/game/types.h"

extern u8 D_8009FA8C[];

void func_8005A044(void)
{
    u8 *cursor;
    s32 remaining;

    remaining = 1;
    cursor = &D_8009FA8C[1];
    do {
        *cursor-- = 0;
    } while (--remaining >= 0);
}
