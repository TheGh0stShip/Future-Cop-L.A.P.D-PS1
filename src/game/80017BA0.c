#include "game/types.h"

extern s32 D_8009FA9C;
extern s32 D_8009F590;

extern void func_8001E2C0(s32 block);

void func_80017BA0(void)
{
    if (D_8009FA9C != 0) {
        func_8001E2C0(D_8009FA9C);
    }
    if (D_8009F590 != 0) {
        func_8001E2C0(D_8009F590);
    }
    D_8009FA9C = 0;
    D_8009F590 = 0;
}
