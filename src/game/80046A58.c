#include "game/types.h"

extern u8 D_800A1830[];

extern s32 func_80045494(s32 id, u8 *table, s32 limit);
extern s32 func_80046B04(s32 arg0, s32 arg1, s32 arg2, s32 slot);

s32 func_80046A58(s32 arg0, s32 arg1, s32 arg2, s32 id)
{
    s32 handle;
    s32 slot;

    handle = 0;
    slot = func_80045494(id, D_800A1830, 0x47);
    if (slot != 0xFF) {
        handle = func_80046B04(arg0, arg1, arg2, slot);
    }
    return handle;
}
