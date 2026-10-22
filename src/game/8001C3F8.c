#include "game/types.h"

s32 func_8001C3F8(u8 *a, u8 *b)
{
    if (b[0] > a[0]) {
        return 1;
    }
    if (b[0] < a[0]) {
        return -1;
    }
    if (b[1] > a[1]) {
        return 1;
    }
    if (b[1] < a[1]) {
        return -1;
    }
    if (b[2] > a[2]) {
        return 1;
    }
    if (b[2] < a[2]) {
        return -1;
    }
    return 0;
}
