#include "game/types.h"

extern s32 D_8009F214;

s32 func_80019720(void)
{
    u32 high;
    u32 low;

    high = ((u32)D_8009F214 >> 16) * 16807;
    low = (D_8009F214 & 0xFFFF) * 16807;
    D_8009F214 = low + ((high & 0x7FFF) << 16) + (high >> 15);
    if (D_8009F214 < 0) {
        D_8009F214 -= 0x7FFFFFFF;
    }
    return D_8009F214;
}
