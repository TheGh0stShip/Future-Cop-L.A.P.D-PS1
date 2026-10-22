#include "game/types.h"

struct Unk4B180Shape {
    u8 unk00[0x0C];
    s32 x;
    s32 y;
    s32 z;
    s16 radius;
};

struct Unk4B180Hit {
    s32 unk00;
    s32 x;
    s32 y;
    s32 z;
    u8 unk10[0x30];
    s16 strength;
    u8 unk42[7];
    u8 mode;
    u8 unk4A[6];
};

struct Unk4B180 {
    u8 unk00[0x0F];
    u8 flags0F;
    u8 flags10;
    u8 unk11[0x0B];
    void *owner;
    u8 unk20[0x44];
    u8 kind;
    u8 unk65[0x1B];
    struct Unk4B180Shape *shape;
};

extern s32 func_8001A314(s32 *vector);
extern s32 func_8003E984(void *owner, s32 arg1, u8 kind, struct Unk4B180Hit *hit, s32 arg4);

void func_8004B180(struct Unk4B180 *object)
{
    struct Unk4B180Hit hit;
    s32 length;

    if (object->shape->radius >= 0) {
        return;
    }
    object->flags0F |= 1;
    if (object->kind == 0) {
        return;
    }
    hit.x = object->shape->x;
    hit.y = object->shape->y;
    hit.z = object->shape->z;
    length = func_8001A314(&hit.x);
    if (hit.y < 0) {
        hit.y = -hit.y;
    }
    hit.strength = length >> 12;
    hit.mode = 2;
    func_8003E984(object->owner, 0, object->kind, &hit, (object->flags10 >> 1) & 1);
}
