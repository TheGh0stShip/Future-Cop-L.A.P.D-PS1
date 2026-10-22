#include "game/types.h"

extern u8 D_800A1830[];
extern u8 D_800A1880[];
extern u8 D_800A18D0[];
extern u8 D_8009F75C;

extern void func_80045504(s32 index);

void func_80044EF8(void)
{
    s32 i;
    u8 *entry;

    for (i = 0; i < 0x46; i++) {
        D_800A1830[i] = 0xFF;
        D_800A1880[i] = 0xFF;
    }
    entry = D_800A18D0;
    for (i = 0x3B; i >= 0; i--) {
        *entry++ = 0xFF;
    }
    D_8009F75C = 0;
    for (i = 0; i < 0x6E; i++) {
        func_80045504(i);
    }
}
