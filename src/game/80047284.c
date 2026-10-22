#include "game/types.h"

extern u8 D_800A1880[];

extern s32 func_80045494(s32 id, u8 *table, s32 limit);
extern s32 func_80046F9C(s32 arg0, s32 arg1, u32 index, s32 arg3, s32 arg4);

void func_80047284(s32 id, s32 arg1, s32 arg2)
{
    s32 slot;

    slot = func_80045494(id, D_800A1880, 0x45);
    if (slot != 0xFF) {
        func_80046F9C(arg1, arg2, slot, 0, 1);
    }
}
