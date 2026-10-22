#include "game/types.h"

extern void *D_800A22F0[];

extern void func_8005DFF0(void *effect);

void func_8005E160(void)
{
    s32 i;

    for (i = 0; i < 4; i++) {
        if (D_800A22F0[i] != 0) {
            func_8005DFF0(D_800A22F0[i]);
        }
    }
}
