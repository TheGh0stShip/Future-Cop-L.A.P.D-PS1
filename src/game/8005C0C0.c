#include "game/types.h"

struct Unk5C0C0Target {
    u8 unk00[8];
    s32 id;
};

struct Unk5C0C0Node {
    struct Unk5C0C0Node *next;
    struct Unk5C0C0Target *first;
    struct Unk5C0C0Target *second;
    s32 unk0C;
    s32 firstId;
    s32 secondId;
    u8 unk18[0x10];
    u8 flags;
};

struct Unk5C0C0Pool {
    void *storage;
    struct Unk5C0C0Node *active;
    struct Unk5C0C0Node *free;
};

extern void func_8003BF1C(struct Unk5C0C0Pool *pool, struct Unk5C0C0Node *previous,
                          struct Unk5C0C0Node *node);

void func_8005C0C0(struct Unk5C0C0Pool *pool, void *owner)
{
    struct Unk5C0C0Node *node;
    struct Unk5C0C0Node *next;
    struct Unk5C0C0Node *previous;

    previous = 0;
    for (node = pool->active; node != 0; node = next) {
        next = node->next;
        if (node->flags & 1) {
            func_8003BF1C(pool, previous, node);
            continue;
        }
        if (node->first != 0 && node->firstId != node->first->id) {
            node->first = 0;
        }
        previous = node;
        if (node->second != 0 && node->secondId != node->second->id) {
            node->second = 0;
        }
    }
}
