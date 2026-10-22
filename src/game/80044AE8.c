#include "game/types.h"

void func_80044AE8(u32 code, u32 key, s32 *shift, u32 *bits)
{
    if (key == (code & 0x3FF) && *shift < 0x1A) {
        code &= 0xFC00;
        code >>= 10;
        code <<= *shift;
        *bits |= code;
        *shift += 6;
    }
}
