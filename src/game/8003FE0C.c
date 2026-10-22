#include "game/types.h"

struct Unk3FD74Source;

struct Unk3FD74Node {
    struct Unk3FD74Node *next;
    u8 type;
    u8 unk05[3];
    s32 arg08;
    u8 unk0C[4];
    struct Unk3FD74Source *source;
    s32 sourceValue;
    s32 arg18;
    u8 flags;
    u8 unk1D[3];
};

struct Unk3FE0CObject {
    u8 unk00[0x44];
    struct Unk3FD74Node *owner;
};

extern struct Unk3FE0CObject *func_8003D680(struct Unk3FD74Source *source,
                                            u8 type, s32 arg08, s32 arg18,
                                            s32 arg4, s32 arg5);
extern s32 func_8003E984(struct Unk3FE0CObject *object, s32 arg1, u8 type,
                         s32 arg18, s32 arg08);

void func_8003FE0C(struct Unk3FD74Node *node)
{
    struct Unk3FE0CObject *object;

    object = func_8003D680(node->source, node->type, node->arg08,
                           node->arg18, 0, 1);
    if (object != 0) {
        object->owner = node;
        if (func_8003E984(object, 0, node->type, node->arg18,
                          node->arg08) != 0) {
            node->flags |= 1;
        }
    }
}
