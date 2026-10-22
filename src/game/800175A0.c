#include "game/types.h"

extern void func_8001428C(s32 arg0, s32 arg1);
extern void func_80017058(s32 arg0, s32 arg1, s32 arg2);

s32 func_800175A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    func_8001428C(arg0, arg2);
    func_80017058(arg0, arg3, arg1);
    return 1;
}
