#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk73500 {
    void *coord;
    void *owner;
    s32 ownerId;
    struct Vec3s32 position;
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
};

extern void *func_80017EBC(s32 size, s32 tag);

struct Unk73500 *func_80073500(struct Vec3s32 *position, s16 arg1, s16 arg2, s16 arg3)
{
    struct Unk73500 *node;

    node = func_80017EBC(0x20, 1);
    if (node == 0) {
        return 0;
    }
    node->unk18 = arg3;
    node->unk1A = arg1;
    node->coord = 0;
    node->owner = 0;
    node->unk1C = arg2;
    node->position = *position;
    return node;
}
