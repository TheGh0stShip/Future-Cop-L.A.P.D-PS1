#include "game/types.h"

extern u8 D_800A1880[];

extern s32 func_80045494(s32 id, u8 *table, s32 limit);
extern s32 func_80046F9C(s32 arg0, s32 arg1, u32 index, s32 arg3, s32 arg4);

s32 func_80046C84(s32 id, s32 arg1)
{
    s32 handle;
    s32 slot;

    handle = 0;
    slot = func_80045494(id, D_800A1880, 0x45);
    if (slot != 0xFF) {
        handle = func_80046F9C(handle, handle, slot, arg1, 1);
    }
    return handle;
}
