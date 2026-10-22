#include "game/types.h"

extern s16 D_8009E0A0[];

s32 func_80095C30(s32 x, s32 y)
{
    s32 result;
    s32 negateY;
    s32 negateX;
    s32 index;

    negateY = 0;
    negateX = 0;
    if (y < 0) {
        negateY = 1;
        y = -y;
    }
    if (x < 0) {
        negateX = 1;
        x = -x;
    }
    if (y == 0 && x == 0) {
        return 0;
    }
    if (x < y) {
        if (x & 0x7FE00000) {
            index = x / (y >> 10);
        } else {
            index = (x << 10) / y;
        }
        result = D_8009E0A0[index];
    } else {
        if (y & 0x7FE00000) {
            index = y / (x >> 10);
        } else {
            index = (y << 10) / x;
        }
        result = 0x400 - D_8009E0A0[index];
    }
    if (negateY) {
        result = 0x800 - result;
    }
    if (negateX) {
        result = -result;
    }
    return result;
}
