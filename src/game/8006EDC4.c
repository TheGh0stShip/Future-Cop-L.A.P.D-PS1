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
extern void func_8003BE2C(struct Pool *pool, s32 size, s32 count);

void func_8006EDC4(void)
{
    func_8003BE2C(&D_800A3538, 0x48, 0xA);
}
