#include "game/types.h"

struct Unk42764Node {
    struct Unk42764Node *next;
};

struct Unk42764Owner {
    u8 unk00[0x64];
    struct Unk42764Node *free;
    struct Unk42764Node *active;
    u8 unk6C[0x38];
    u16 count;
};

struct Unk42764Node *func_80042764(struct Unk42764Owner *owner)
{
    struct Unk42764Node *node;

    node = owner->free;
    if (node != 0) {
        owner->free = node->next;
    }
    return node;
}
