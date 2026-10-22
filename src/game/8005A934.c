#include "game/types.h"

struct Unk5A934 {
    u8 unk00[0x50];
    s32 x;
    s32 y;
    s32 z;
    u8 unk5C[0x54];
    s16 heading;
};

extern s32 func_80095C30(s32 x, s32 z);

s32 func_8005A934(struct Unk5A934 *object, s32 z, s32 x, s32 *angle)
{
    s32 dz;
    s32 dx;
    s32 direction;

    dz = z - object->z;
    dx = x - object->x;
    if ((((dz >> 31) + dz) ^ (dz >> 31)) > 0x2000 ||
        (((dx >> 31) + dx) ^ (dx >> 31)) > 0x2000) {
        direction = func_80095C30(dx, dz);
        direction = (direction << 12) >> 12;
        direction -= object->heading;
        direction -= 0x400;
        *angle = direction;
        return 1;
    }
    return 0;
}
