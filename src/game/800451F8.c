#include "game/types.h"

extern s32 D_8009F330;

void func_800451F8(s32 volume)
{
    if (volume >= 0) {
        if (volume < 0x80) {
            D_8009F330 = volume;
        } else {
            D_8009F330 = 0x7F;
        }
    } else {
        D_8009F330 = 0;
    }
}
