#include "game/types.h"

extern s32 D_8009F338;

void func_80045294(s32 volume)
{
    if (volume >= 0) {
        if (volume < 0x80) {
            D_8009F338 = volume;
        } else {
            D_8009F338 = 0x7F;
        }
    } else {
        D_8009F338 = 0;
    }
}
