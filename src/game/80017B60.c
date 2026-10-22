#include "game/types.h"

extern void *D_8009F9E4;

extern void *func_80017B28(s32 id);

void func_80017B60(s32 id)
{
    D_8009F9E4 = func_80017B28(id);
}
