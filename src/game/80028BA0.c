#include "../../include/game/types.h"

void func_80028BA0(u32 *destination, const u32 *source, u32 byte_count)
{
    u32 count = byte_count >> 2;

    while (count != 0) {
        *destination++ = *source++;
        count--;
    }
}
