#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

extern s32 D_8009F324;
extern u8 D_8009F328;
extern s16 D_8009F710;
extern s16 D_8009F718;
extern s16 D_8009F71C;

extern void func_800410BC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_80041428(s32 arg0, s32 arg1, s32 arg2, s32 context, struct Vec3s32 *position,
                   s32 height, s32 arg6)
{
    D_8009F710 = height >> 4;
    D_8009F718 = position->x >> 4;
    D_8009F328 = 1;
    D_8009F324 = context;
    D_8009F71C = position->z >> 4;
    func_800410BC(arg0, arg1, arg2, arg6);
    D_8009F328 = 0;
    D_8009F324 = 0;
}
