#include "game/types.h"

struct Unk7938 {
    u8 unk00[0x10];
    u8 active;
    u8 unk11[0x0B];
};

extern u8 D_800A18D0[];
extern struct Unk7938 D_800A7938[];

u8 func_80045494(s32 id, u8 *table, s32 limit)
{
    s32 bias;
    u8 slot;

    if (id >= 0x46) {
        table = D_800A18D0;
        limit = 0x82;
        bias = -0x46;
    } else {
        bias = 0;
    }
    if (id > 0 && id < limit) {
        slot = table[id + bias];
        if (D_800A7938[slot].active != 0) {
            return slot;
        }
    }
    return 0xFF;
}
