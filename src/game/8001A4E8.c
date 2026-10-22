#include "game/types.h"

s32 func_8001A4E8(s32 *a, s32 *b)
{
    s32 x;
    s32 y;
    s32 z;

    x = a[0] - b[0];
    if (x < 0) {
        x = -x;
    }
    y = a[1] - b[1];
    if (y < 0) {
        y = -y;
    }
    z = a[2] - b[2];
    if (z < 0) {
        z = -z;
    }
    if (x < y) {
        x ^= y;
        y ^= x;
        x ^= y;
    }
    if (x < z) {
        x ^= z;
        z ^= x;
        x ^= z;
    }
    y += z;
    return x + (y >> 2);
}
