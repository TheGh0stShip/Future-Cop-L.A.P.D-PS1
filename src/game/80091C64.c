#include "../../include/game/types.h"

extern s32 D_8009DD30[];

void func_80091C64(u32 index, s32 value)
{
    if (D_8009DD30[index] != value) {
        D_8009DD30[index] = value;
    }
}
