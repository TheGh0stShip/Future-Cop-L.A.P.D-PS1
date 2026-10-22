#include "game/types.h"

struct Unk3F428Node;

struct Unk3F428Ops {
    s32 unk00;
    void (*handler)(s32 message, struct Unk3F428Node *node, s32 arg2, s32 arg3);
};

struct Unk3F428Def {
    u8 unk00[0x48];
    struct Unk3F428Ops *ops;
};

struct Unk3F428Node {
    struct Unk3F428Node *next;
    u8 unk04[0x18];
    struct Unk3F428Def *def;
};

void func_8003F428(struct Unk3F428Node *node, s32 arg)
{
    while (node != 0) {
        if (node->def != 0 && node->def->ops != 0) {
            node->def->ops->handler(1, node, arg, 0);
        }
        node = node->next;
    }
}
