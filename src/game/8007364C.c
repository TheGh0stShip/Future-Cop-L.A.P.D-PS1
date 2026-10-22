#include "game/types.h"

struct Unk7364C {
    struct Unk7364C *next;
    u8 unk04[0x0A];
    u16 flags;
};

extern struct Unk7364C *D_8009F38C;

extern void func_80071A68(struct Unk7364C *node);
extern void func_80071B40(struct Unk7364C *node, s32 arg);

void func_8007364C(s32 arg)
{
    struct Unk7364C *node;
    register struct Unk7364C *previous __asm__("$18");
    register struct Unk7364C *next __asm__("$17");
    u16 flags;

    previous = 0;
    for (node = D_8009F38C; node != 0; node = next) {
        flags = node->flags;
        next = node->next;
        if (flags & 0x1000) {
            if (previous != 0) {
                previous->next = next;
            } else {
                D_8009F38C = next;
            }
            node->next = 0;
            func_80071A68(node);
        } else if (flags & 0x800) {
            func_80071B40(node, arg);
            previous = node;
        } else {
            node->flags = flags | 0x800;
            previous = node;
        }
    }
}
