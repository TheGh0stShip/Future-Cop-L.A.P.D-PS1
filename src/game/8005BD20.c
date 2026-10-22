#include "game/types.h"

struct Unk5BD20Target {
    u8 unk00[8];
    s32 id;
};

struct Unk5BD20Node {
    struct Unk5BD20Node *next;
    struct Unk5BD20Target *first;
    struct Unk5BD20Target *second;
    s32 unk0C;
    s32 firstId;
    s32 secondId;
    u8 unk18[0x12];
    u8 flags;
};

struct Unk5BD20Pool {
    void *storage;
    struct Unk5BD20Node *active;
    struct Unk5BD20Node *free;
};

extern void func_8003BF1C(struct Unk5BD20Pool *pool, struct Unk5BD20Node *previous,
                          struct Unk5BD20Node *node);

void func_8005BD20(struct Unk5BD20Pool *pool, void *owner)
{
    struct Unk5BD20Node *node;
    struct Unk5BD20Node *next;
    struct Unk5BD20Node *previous;

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
