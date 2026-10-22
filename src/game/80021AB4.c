#include "../../include/game/types.h"

struct FlagState {
    u8 pad_00[0x18];
    u32 flags;
};

extern struct FlagState D_800A07A8;

void func_80021AB4(u32 flags)
{
    D_800A07A8.flags |= flags;
}
