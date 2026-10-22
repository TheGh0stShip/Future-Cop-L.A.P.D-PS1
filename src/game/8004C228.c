#include "game/types.h"

struct Unk4C228 {
    struct Unk4C228 *next;
    u8 unk04[0x0B];
    u8 flags0F;
    u8 unk10[0x6B];
    u8 flags7B;
};

extern void func_8004AFC0(struct Unk4C228 *node, s32 arg);
extern void func_8004BEE8(struct Unk4C228 *node, s32 arg);

void func_8004C228(struct Unk4C228 *node, s32 arg)
{
    while (node != 0) {
        if (!(node->flags0F & 8)) {
            if (node->flags7B & 8) {
                func_8004AFC0(node, arg);
            } else if (!(node->flags7B & 0x10)) {
                func_8004BEE8(node, arg);
            }
        }
        node = node->next;
    }
}
