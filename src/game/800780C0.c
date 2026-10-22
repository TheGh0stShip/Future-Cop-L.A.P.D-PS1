#include "game/types.h"

extern u8 D_8009B3EC[];
extern u8 *D_8009F878;
extern s16 D_8009F87C;

void func_800780C0(void)
{
    D_8009F878 = D_8009B3EC;
    D_8009F87C = 0x40;
}
