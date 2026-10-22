#include "game/types.h"

extern u8 D_800A2290[][8];

extern u32 func_80019668(void);

void func_80056924(void)
{
    s32 i;

    for (i = 0; i < 8; i++) {
        D_800A2290[0][i] = (func_80019668() & 7) + 7;
        D_800A2290[1][i] = (func_80019668() & 7) + 7;
    }
}
