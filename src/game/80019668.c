#include "game/types.h"

extern s32 D_8009F5A8;
extern s32 D_800A0480[];

s32 func_80019668(void)
{
    s32 i;
    s32 j;

    i = D_8009F5A8 - 1;
    j = D_8009F5A8 + 2;
    if (i < 0) {
        i = 0x13;
    } else if (j >= 0x14) {
        j = D_8009F5A8 - 0x12;
    }
    D_8009F5A8 = i;
    return D_800A0480[i] += D_800A0480[j];
}
