#include "game/types.h"

struct Unk734B0 {
    struct Unk734B0 *next;
};

extern struct Unk734B0 *D_8009F38C;

extern void func_80073BA8(struct Unk734B0 *node);

void func_800734BC(void)
{
    struct Unk734B0 *node;
    struct Unk734B0 *next;

    node = D_8009F38C;
    while (node != 0) {
        next = node->next;
        func_80073BA8(node);
        node = next;
    }
    D_8009F38C = 0;
}
