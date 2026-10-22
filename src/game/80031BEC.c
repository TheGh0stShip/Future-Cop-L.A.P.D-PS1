#include "game/types.h"

struct Unk31Node {
    s16 x;
    s16 unk02;
    s16 z;
    s16 unk06;
    s32 id;
    s16 extentX;
    s16 unk0E;
    s16 extentZ;
    s16 unk12;
    u8 flags;
    u8 unk15;
    s16 unk16;
    u8 unk18[0x0C];
    struct Unk31Node *next;
};

extern struct Unk31Node *D_8009F6F8;

void func_80031BEC(s32 id, s32 kind, s32 mode)
{
    struct Unk31Node *node;

    for (node = D_8009F6F8; node != 0; node = node->next) {
        if (node->id == id) {
            if (kind == 0x7C) {
                if (mode == 1) {
                    node->unk16 = 0;
                    node->flags &= ~0x10;
                } else if (mode == 0) {
                    node->flags |= 0x10;
                } else {
                    node->unk15 = 1;
                }
            }
            return;
        }
    }
}
