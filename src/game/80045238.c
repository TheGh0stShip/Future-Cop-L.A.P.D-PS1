#include "game/types.h"

extern s32 D_8009F334;

extern void func_80024730(s32 volume);

void func_80045238(s32 volume)
{
    if (volume >= 0) {
        if (volume < 0x80) {
            D_8009F334 = volume;
        } else {
            D_8009F334 = 0x7F;
        }
    } else {
        D_8009F334 = 0;
    }
    func_80024730(D_8009F334);
}
