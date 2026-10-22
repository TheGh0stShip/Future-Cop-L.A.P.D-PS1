#include "game/types.h"

extern void func_80048A14(s32 value, s32 *source);

void func_80048E9C(s32 *source)
{
    func_80048A14(*source, source);
}
