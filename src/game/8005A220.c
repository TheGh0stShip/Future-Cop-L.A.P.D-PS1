#include "game/types.h"

extern void **D_8009F7D8;
extern s32 D_8009F7DC;

extern void func_800181D4(void *block);

void func_8005A220(void)
{
    s32 i;

    if (D_8009F7D8 != 0) {
        for (i = D_8009F7DC - 1; i >= 0; i--) {
            func_800181D4(D_8009F7D8[i]);
        }
        func_800181D4(D_8009F7D8);
        D_8009F7D8 = 0;
    }
    D_8009F7DC = 0;
}
