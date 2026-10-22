#include "game/types.h"

struct Unk73590Owner {
    u8 unk00[8];
    s32 id;
    u8 unk0C[0x1C];
    u8 coord[4];
};

struct Unk73590 {
    void *coord;
    struct Unk73590Owner *owner;
    s32 ownerId;
    u8 kind;
    u8 unk0D[0x0B];
    s16 unk18;
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
};

extern void *func_80017EBC(s32 size, s32 tag);

struct Unk73590 *func_80073590(struct Unk73590Owner *owner, void *coord, u8 kind, s16 arg3,
                               s16 arg4, s16 arg5)
{
    struct Unk73590 *node;

    node = func_80017EBC(0x20, 1);
    if (node == 0) {
        return 0;
    }
    node->unk18 = arg5;
    node->unk1A = arg3;
    node->unk1C = arg4;
    if (coord == 0 && owner != 0) {
        coord = owner->coord;
    }
    node->coord = coord;
    node->kind = kind;
    node->owner = owner;
    if (owner != 0) {
        node->ownerId = owner->id;
        if (coord == 0) {
            node->coord = owner->coord;
        }
    }
    return node;
}
