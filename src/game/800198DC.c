#include "game/types.h"

extern void *D_8009F5B0;

extern void func_800181D4(void *block);

void func_800198DC(void)
{
    if (D_8009F5B0 != 0) {
        func_800181D4(D_8009F5B0);
    }
}
