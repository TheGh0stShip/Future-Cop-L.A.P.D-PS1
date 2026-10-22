#include "game/types.h"

struct PoolNode {
    struct PoolNode *next;
};

struct Pool {
    void *storage;
    struct PoolNode *active;
    struct PoolNode *free;
};

extern void func_800181D4(void *block);

void func_8003BDEC(struct Pool *pool)
{
    if (pool->storage != 0) {
        func_800181D4(pool->storage);
        pool->storage = 0;
    }
}
