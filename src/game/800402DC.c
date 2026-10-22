#include "game/types.h"

struct Unk402DCOwner {
    u8 unk00[8];
    s32 unk08;
};

extern void *D_800A7040[][0x100];
extern u8 D_8009F31C;

extern u32 *func_8003B3D8(u32 column, u32 row);
extern void *func_8003D680(struct Unk402DCOwner *owner, u8 column, s32 row, s32 arg3,
                           s32 arg4, u8 arg5);
extern s32 func_8003E984(void *object, s32 arg1, u8 column, s32 arg3, s32 row);
extern void *func_8003FD74(s32 column, s32 row, struct Unk402DCOwner *owner, s32 arg3);

s32 func_800402DC(struct Unk402DCOwner *owner, u8 column, s32 arg2, s32 row, s32 arg4)
{
    if (D_800A7040[row][column] == 0) {
        return 0;
    }
    if ((*(u16 *)func_8003B3D8(column, row) & 5) == 4) {
        if (owner == 0) {
            return 0;
        }
        if (owner->unk08 != 0) {
            func_8003FD74(column, row, owner, 0);
        }
        return 0;
    }
    return func_8003E984(func_8003D680(owner, column, row, arg2, arg4, D_8009F31C), 0,
                         column, arg2, row);
}
