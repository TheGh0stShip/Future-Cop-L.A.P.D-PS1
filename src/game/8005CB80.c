#include "game/types.h"

struct Unk5CB80Node {
    struct Unk5CB80Node *next;
    void *first;
    void *second;
    u8 unk0C[0x1C];
    u8 flags;
};

struct Unk5CB80Pool {
    void *storage;
    struct Unk5CB80Node *active;
    struct Unk5CB80Node *free;
};

struct Unk5CB80Node *func_8005CB80(struct Unk5CB80Pool *pool, void *target)
{
    struct Unk5CB80Node *node;

    for (node = pool->active; node != 0; node = node->next) {
        if (node->second == target && !(node->flags & 2)) {
            return node;
        }
    }
    return 0;
}
