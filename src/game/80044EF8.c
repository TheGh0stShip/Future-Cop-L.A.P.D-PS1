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
    u8 *other;
    u8 value;

    entry = D_800A1830;
    other = D_800A1880;
    i = 0;
    do {
        i++;
        *entry++ = 0xFF;
        *other++ = 0xFF;
    } while (i < 0x46);
    entry = D_800A18D0;
    value = 0xFF;
    for (i = 0x3B; i >= 0; i--) {
        *entry++ = value;
    }
    D_8009F75C = 0;
    for (i = 0; i < 0x6E; i++) {
        func_80045504(i);
    }
}
