#include "game/types.h"

extern u16 D_8009A234[];

void func_8001846C(u32 angle, s32 *sine, s32 *cosine)
{
    s32 index;
    s32 s;
    s32 c;
    s32 quadrant;

    angle &= 0xFFF;
    index = angle & 0x3FF;
    s = D_8009A234[index] << 1;
    c = D_8009A234[0x400 - index] << 1;
    quadrant = (s32)angle >> 10;
    switch (quadrant) {
    case 0:
        *sine = s;
        *cosine = c;
        break;
    case 1:
        *sine = c;
        *cosine = -s;
        break;
    case 2:
        *sine = -s;
        *cosine = -c;
        break;
    case 3:
        *sine = -c;
        *cosine = s;
        break;
    }
}
