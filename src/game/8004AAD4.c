#include "game/types.h"

struct Unk4AADParent {
    u8 unk00[0x39];
    u8 flags;
    u16 count;
};

struct Unk4AADNode {
    u8 unk00[0x28];
    u8 flags;
    u8 unk29[0x1F];
    struct Unk4AADNode *next;
    struct Unk4AADParent *parent;
    u8 unk50[8];
};

struct Unk4AADOwner {
    u8 unk00[0x84];
    struct Unk4AADNode *head;
};

extern struct Unk4AADNode *D_8009F798;

struct Unk4AADNode *func_8004AAD4(struct Unk4AADOwner *owner, struct Unk4AADNode *previous,
                                  struct Unk4AADNode *node)
{
    if (previous != 0) {
        previous->next = node->next;
    } else {
        owner->head = node->next;
    }
    node->next = D_8009F798;
    D_8009F798 = node;
    if (node->flags & 0x80) {
        node->parent->count--;
        node->parent->flags |= 1;
    }
    return owner->head;
}
