#include "game/types.h"

void func_80044AE8(u32 code, u32 key, s32 *shift, u32 *bits)
{
    if ((code & 0x3FF) == key && *shift < 0x1A) {
        *bits |= ((code & 0xFC00) >> 10) << *shift;
        *shift += 6;
    }
}
