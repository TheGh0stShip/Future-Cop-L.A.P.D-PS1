#include "game/types.h"

struct Unk5A934 {
    u8 unk00[0x50];
    s32 x;
    s32 y;
    s32 z;
    u8 unk5C[0x54];
    s16 heading;
};

extern int abs(int value);
extern s32 func_80095C30(s32 x, s32 z);

s32 func_8005A934(struct Unk5A934 *object, s32 z, s32 x, s32 *angle)
{
    s32 dz;
    s32 dx;

    dz = z - object->z;
    dx = x - object->x;
    if (abs(dz) > 0x2000 || abs(dx) > 0x2000) {
        *angle = ((func_80095C30(dx, dz) << 12) >> 12) - object->heading - 0x400;
        return 1;
    }
    return 0;
}
