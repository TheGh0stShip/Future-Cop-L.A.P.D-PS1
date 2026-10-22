#include "../../include/game/types.h"

extern u32 *D_8009EEB8;

void func_80097940(void)
{
    *D_8009EEB8 = (*D_8009EEB8 & 0xF0FFFFFFU) | 0x20000000U;
}
