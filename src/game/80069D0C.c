#include "game/types.h"

extern s32 *D_8009F9A4;

void func_80069D0C(s32 amount)
{
    s32 i;
    s32 *timer;
    register s32 value __asm__("$3");

    i = 0;
    timer = D_8009F9A4;
    for (; i < 0x20; i++, timer++) {
        if (*timer > 0) {
            value = *timer - amount;
            if (value < 5) {
                value = 0;
            }
            *timer = value;
        }
    }
}
