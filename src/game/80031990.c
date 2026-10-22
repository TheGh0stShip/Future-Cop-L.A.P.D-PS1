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
extern void func_800181D4(void *block);

void func_80031990(void)
{
    struct Unk31Node *node;
    struct Unk31Node *next;

    node = D_8009F6F8;
    while (node != 0) {
        next = node->next;
        func_800181D4(node);
        node = next;
    }
    D_8009F6F8 = 0;
}
