#include "../../include/game/types.h"

void *func_80021800(void *base, u32 index)
{
    u32 offset = *(u32 *)((u8 *)base + 0x10 + index * 4);

    if (offset == 0) {
        return 0;
    }
    return (u8 *)base + offset;
}
