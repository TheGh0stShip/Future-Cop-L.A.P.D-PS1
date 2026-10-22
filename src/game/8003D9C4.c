#include "../../include/game/types.h"

struct ChildNode {
    u8 pad_00[0x34];
    void *child;
};

struct ParentNode {
    u8 pad_00[0x1C];
    struct ChildNode *node;
    u8 pad_20[0x14];
    void *fallback;
};

void **func_8003D9C4(struct ParentNode *parent)
{
    struct ChildNode *node = parent->node;

    if (node == 0) {
        return &parent->fallback;
    }
    if (node->child != 0) {
        return (void **)((u8 *)node->child + 4);
    }
    return (void **)((u8 *)node + 4);
}
