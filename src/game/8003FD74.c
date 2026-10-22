#include "game/types.h"

struct Unk3FD74Source {
    u8 unk00[8];
    s32 value08;
};

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

extern struct Unk3FD74Node *D_8009F30C;
extern void *func_80017EBC(s32 size, s32 tag);

struct Unk3FD74Node *func_8003FD74(s32 type, s32 arg08,
                                   struct Unk3FD74Source *source, s32 arg18)
{
    struct Unk3FD74Node *node;

    node = func_80017EBC(0x20, 1);
    if (node == 0) {
        return 0;
    }
    node->next = 0;
    node->flags = 0;
    node->type = type;
    node->arg08 = arg08;
    node->source = source;
    if (source != 0) {
        node->sourceValue = source->value08;
    }
    node->arg18 = arg18;
    node->next = D_8009F30C;
    D_8009F30C = node;
    return node;
}
