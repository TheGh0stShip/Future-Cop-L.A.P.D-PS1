#include "game/types.h"

struct PoolNode {
    struct PoolNode *next;
};

struct Pool {
    void *storage;
    struct PoolNode *active;
    struct PoolNode *free;
};

extern struct Pool D_800A3538;
extern void func_8003BDEC(struct Pool *pool);

void func_8006EDF0(void)
{
    func_8003BDEC(&D_800A3538);
}
