#include "game/types.h"

extern s32 D_8009F324;
extern u8 D_8009F329;

extern void func_800410BC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void func_800413EC(s32 arg0, s32 arg1, s32 arg2, s32 context, s32 arg4, s32 mode)
{
    D_8009F324 = context;
    D_8009F329 = mode | 1;
    func_800410BC(arg0, arg1, arg2, arg4);
    D_8009F329 = 0;
    D_8009F324 = 0;
}
