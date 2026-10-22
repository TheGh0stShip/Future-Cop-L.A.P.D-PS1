#include "game/types.h"

struct PoolNode {
    struct PoolNode *next;
};

extern struct PoolNode *D_800A353C;
extern void func_8006F310(struct PoolNode *node, s32 arg);

void func_8006F844(s32 arg)
{
    struct PoolNode *node;

    for (node = D_800A353C; node != 0; node = node->next) {
        func_8006F310(node, arg);
    }
}
