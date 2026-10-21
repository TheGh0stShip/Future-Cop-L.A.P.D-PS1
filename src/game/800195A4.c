#include "../../include/game/types.h"

struct LinkState {
    u8 pad_00[0xC];
    s32 count;
    struct LinkNode *head;
};

struct LinkNode {
    struct LinkNode *next;
};

struct LinkNode *func_800195A4(struct LinkState *state)
{
    struct LinkNode *node = state->head;

    if (node != 0) {
        state->head = node->next;
        state->count--;
    }
    return node;
}
