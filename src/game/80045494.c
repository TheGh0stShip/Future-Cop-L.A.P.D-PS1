#include "game/types.h"

struct Unk7938 {
    u8 unk00[0x10];
    u8 active;
    u8 unk11[0x0B];
};

extern u8 D_800A18D0[];
extern struct Unk7938 D_800A7938[];

s32 func_80045494(s32 id, u8 *table, s32 limit)
{
    register s32 bias __asm__("$2");
    register s32 adjusted __asm__("$3");
    register u8 *entry __asm__("$2");
    register struct Unk7938 *base __asm__("$3");
    register struct Unk7938 *record __asm__("$2");

    if (id >= 0x46) {
        table = D_800A18D0;
        limit = 0x82;
        bias = -0x46;
    } else {
        bias = 0;
    }
    adjusted = id + bias;
    if (id > 0) {
        if (id < limit) {
            entry = &table[adjusted];
            __asm__ volatile ("" : "+r" (entry));
            base = D_800A7938;
            id = *entry;
            record = &base[id];
            if (record->active != 0) {
                goto done;
            }
        }
    }
    id = 0xFF;
done:
    return id;
}
