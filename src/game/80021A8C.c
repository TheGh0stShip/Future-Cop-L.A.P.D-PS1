#include "../../include/game/types.h"

struct EventFlags {
    u8 pad[0x14];
    u32 pending;
    u32 acknowledged;
};

extern struct EventFlags D_800A07A8;

void func_80021A8C(u32 flags)
{
    D_800A07A8.pending &= ~flags;
    D_800A07A8.acknowledged |= flags;
}
