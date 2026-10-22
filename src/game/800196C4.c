#include "game/types.h"

extern s32 D_8009F5AC;
extern s32 D_800A04D0[];

s32 func_800196C4(void)
{
    s32 i;
    s32 j;

    i = D_8009F5AC - 1;
    j = D_8009F5AC + 2;
    if (i < 0) {
        i = 0x13;
    } else if (j >= 0x14) {
        j = D_8009F5AC - 0x12;
    }
    D_8009F5AC = i;
    return D_800A04D0[i] += D_800A04D0[j];
}
