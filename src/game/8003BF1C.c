#include "game/types.h"

struct PoolNode {
    struct PoolNode *next;
};

struct Pool {
    void *storage;
    struct PoolNode *active;
    struct PoolNode *free;
};

void func_8003BF1C(struct Pool *pool, struct PoolNode *previous,
                   struct PoolNode *node)
{
    if (previous != 0) {
        previous->next = node->next;
    } else {
        pool->active = node->next;
    }
    node->next = pool->free;
    pool->free = node;
}
