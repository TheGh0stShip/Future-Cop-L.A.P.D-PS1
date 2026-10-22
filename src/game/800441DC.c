#include "game/types.h"

struct Unk441DC {
    struct Unk441DC *next;
    u8 unk04[0x0B];
    u8 flags0F;
    u8 unk10[0x7D];
    u8 flags8D;
};

extern void func_80043CA8(struct Unk441DC *node, s32 arg);
extern void func_800437E8(struct Unk441DC *node, s32 arg);

void func_800441DC(struct Unk441DC *node, s32 arg)
{
    while (node != 0) {
        if (!(node->flags0F & 8)) {
            if (node->flags8D & 1) {
                func_80043CA8(node, arg);
            } else {
                func_800437E8(node, arg);
            }
        }
        node = node->next;
    }
}
