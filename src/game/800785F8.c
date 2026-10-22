#include "game/types.h"

extern s32 D_8009F890[];
extern s32 D_8009F898[];
extern s32 D_8009F8A0[];
extern s32 D_8009F8A8[];

void func_800785F8(void)
{
    u32 i;

    for (i = 0; i < 2; i++) {
        D_8009F890[i] = 0;
        D_8009F898[i] = 0;
        D_8009F8A0[i] = 0;
        D_8009F8A8[i] = 0;
    }
}
