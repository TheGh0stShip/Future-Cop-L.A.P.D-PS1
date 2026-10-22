#include "game/types.h"

extern void func_80021A8C(u32 mask);

void func_8002238C(s32 channel)
{
    func_80021A8C(1 << channel);
}
