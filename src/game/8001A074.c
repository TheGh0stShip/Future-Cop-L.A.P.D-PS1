#include "game/types.h"

s32 func_8001A074(s32 *vector)
{
    s32 a;
    s32 b;
    s32 sum;

    a = vector[0];
    if (a < 0) {
        a = -a;
    }
    b = vector[1];
    if (b < 0) {
        b = -b;
    }
    if (a < b) {
        a ^= b;
        b ^= a;
        a ^= b;
    }
    sum = a + b;
    if ((a >> 1) < b) {
        sum = (sum << 6) + (sum << 3) + (sum << 1) + a + (b << 5);
        b = a - b;
        a = sum >> 7;
    }
    return a + (b >> 2) - (b >> 6);
}
