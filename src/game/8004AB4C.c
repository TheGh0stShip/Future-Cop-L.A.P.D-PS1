#include "game/types.h"

struct Unk4AADNode {
    u8 unk00[0x48];
    struct Unk4AADNode *next;
    u8 unk4C[0x0C];
};

extern struct Unk4AADNode *D_8009F798;
extern struct Unk4AADNode *D_8009F79C;
extern s32 D_8009F9DC;

extern void *func_80017EBC(s32 size, s32 tag);
extern void *func_8008CD20(void *buffer, s32 value, s32 size);

void func_8004AB4C(void)
{
    struct Unk4AADNode *pool;
    s32 i;

    D_8009F9DC = 0;
    pool = func_80017EBC(0x44C0, 2);
    D_8009F798 = pool;
    D_8009F79C = pool;
    if (pool != 0) {
        func_8008CD20(pool, 0, 0x44C0);
        for (i = 0; i < 0xC7; i++) {
            D_8009F798[i].next = &D_8009F798[i + 1];
        }
    }
}
