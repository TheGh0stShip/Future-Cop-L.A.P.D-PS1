#include "game/types.h"

struct Unk5BCD4Node {
    struct Unk5BCD4Node *next;
    void *first;
    void *second;
    u8 unk0C[0x1E];
    u8 flags;
};

struct Unk5BCD4Pool {
    void *storage;
    struct Unk5BCD4Node *active;
    struct Unk5BCD4Node *free;
};

struct Unk5BCD4Node *func_8005BCD4(struct Unk5BCD4Pool *pool, void *target)
{
    struct Unk5BCD4Node *node;

    for (node = pool->active; node != 0; node = node->next) {
        if (node->second == target && !(node->flags & 2)) {
            return node;
        }
    }
    return 0;
}
