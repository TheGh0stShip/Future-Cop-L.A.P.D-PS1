#include "game/types.h"

s32 func_80019F54(s32 *vector)
{
    s32 a;
    s32 b;
    s32 c;

    a = vector[0];
    if (a < 0) {
        a = -a;
    }
    b = vector[1];
    if (b < 0) {
        b = -b;
    }
    c = vector[2];
    if (c < 0) {
        c = -c;
    }
    if (a < b) {
        a ^= b;
        b ^= a;
        a ^= b;
    }
    if (a < c) {
        a ^= c;
        c ^= a;
        a ^= c;
    }
    b += c;
    return a + (b >> 2);
}
