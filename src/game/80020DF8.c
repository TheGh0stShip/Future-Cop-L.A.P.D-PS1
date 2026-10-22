#include "game/types.h"

struct CardContext {
    s32 unk00;
    s32 slots[16];
};

extern struct CardContext *D_8009AB08;

extern void func_800207CC(void);

s32 func_80020DF8(u32 slot)
{
    if (slot < 16) {
        if (D_8009AB08->slots[slot] == 0) {
            func_800207CC();
        }
        return D_8009AB08->slots[slot];
    }
    return -1;
}
