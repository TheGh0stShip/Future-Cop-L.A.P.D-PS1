#include "../../include/game/types.h"

extern void func_80019A74(s32 *value);
extern s32 D_8009F9F4;

void func_8004D924(s32 *value)
{
    func_80019A74(value);
    D_8009F9F4 = *value;
}
