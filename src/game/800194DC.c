#include "game/types.h"

struct FixedPool {
    s32 stride;
    u8 *end;
    s32 count;
    s32 available;
    void *free;
};

extern void *func_80017EBC(s32 size, s32 tag);

struct FixedPool *func_800194DC(s32 count, s32 size, s32 tag)
{
    struct FixedPool *pool;
    void **node;
    void *previous;
    s32 stride;
    s32 bytes;

    stride = (size + 3) & ~3;
    bytes = count * stride + 0x14;
    pool = func_80017EBC(bytes, tag);
    if (pool != 0) {
        previous = 0;
        node = (void **)(pool + 1);
        pool->count = count;
        pool->available = count;
        pool->stride = stride;
        pool->end = (u8 *)(bytes + (s32)pool);
        while (count-- > 0) {
            *node = previous;
            previous = node;
            node = (void **)((u8 *)previous + stride);
        }
        pool->free = previous;
    }
    return pool;
}
