#include "game/types.h"

extern s32 D_8009F214;
extern s32 D_8009F5A8;
extern s32 D_8009F5AC;
extern s32 D_800A0480[];
extern s32 D_800A04D0[];

extern s32 func_80019720(void);

void func_800197A8(s32 seed)
{
    s32 i;

    D_8009F214 = seed;
    D_8009F5A8 = 0x14;
    D_8009F5AC = 0x14;
    for (i = 0; i < 0x14; i++) {
        D_800A0480[i] = func_80019720();
    }
    for (i = 0; i < 0x14; i++) {
        D_800A04D0[i] = func_80019720();
    }
}
