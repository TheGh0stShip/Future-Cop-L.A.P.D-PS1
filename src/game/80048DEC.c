#include "game/types.h"

extern s32 D_8009F76C;
extern s32 D_8009F364;
extern u16 D_8009F76A;

extern void func_80048BF8(s32 value);

void func_80048DEC(void)
{
    if (D_8009F76C == 0 && D_8009F364 == 1) {
        D_8009F76A = D_8009F364;
        func_80048BF8(0);
    }
}
