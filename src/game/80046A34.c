#include "game/types.h"

extern void func_80046698(s32 handle);

void func_80046A34(s32 handle)
{
    if (handle != 0) {
        func_80046698(handle);
    }
}
