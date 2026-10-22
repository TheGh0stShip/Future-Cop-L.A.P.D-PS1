#include "game/types.h"

extern u8 D_8009F739;

extern void func_8002A16C(s32 value);
extern void func_80047554(s32 value);

void func_80047690(u8 enable)
{
    if (enable) {
        D_8009F739 = 1;
        func_8002A16C(1);
    } else {
        func_80047554(0);
    }
}
