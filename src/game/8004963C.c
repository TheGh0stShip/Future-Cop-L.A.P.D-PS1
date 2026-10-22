#include "game/types.h"

extern void *D_8009F77C;
extern void *D_8009F780;
extern s32 D_8009F784;

extern void func_800181D4(void *block);
extern void func_80019584(s32 pool);

void func_8004963C(void)
{
    if (D_8009F77C != 0) {
        func_800181D4(D_8009F77C);
    }
    D_8009F77C = 0;
    func_80019584(D_8009F784);
    D_8009F784 = 0;
    if (D_8009F780 != 0) {
        func_800181D4(D_8009F780);
    }
    D_8009F780 = 0;
}
