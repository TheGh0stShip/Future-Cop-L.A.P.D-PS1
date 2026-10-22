#include "game/types.h"

s32 func_8005605C(s32 current, s32 target, s32 steps)
{
    s32 difference;
    s32 sign;
    s32 start;
    s32 i;

    current &= 0xFFF;
    if (current >= 0x800) {
        current -= 0x1000;
    }
    target &= 0xFFF;
    if (target >= 0x800) {
        target -= 0x1000;
    }
    difference = target - current;
    sign = difference >> 31;
    if (((sign + difference) ^ sign) >= 0x800) {
        if (difference > 0) {
            current += 0x1000;
        } else {
            current -= 0x1000;
        }
    }
    start = current;
    for (i = steps; i > 0; i--) {
        current = (current * 31 + target) / 32;
    }
    if (current == start && current != target) {
        if (difference < 0) {
            if (-difference < steps) {
                steps = -difference;
            }
            current -= steps;
        } else {
            if (difference < steps) {
                steps = difference;
            }
            current += steps;
        }
    }
    return current;
}
