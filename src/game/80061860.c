#include "game/types.h"

extern void *D_8009F820;
extern void *D_8009F818[2];
extern void *D_8009F830;
extern void func_800181D4(void *block);

void func_80061860(void)
{
    s32 i;

    if (D_8009F820 != 0) {
        func_800181D4(D_8009F820);
    }
    for (i = 0; i < 2; i++) {
        func_800181D4(D_8009F818[i]);
    }
    func_800181D4(D_8009F830);
}
