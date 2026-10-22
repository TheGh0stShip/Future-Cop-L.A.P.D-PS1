#include "game/types.h"

struct Unk4F0F0Entry {
    s32 unk00;
    u8 flags;
    u8 unk05[7];
};

extern struct Unk4F0F0Entry *D_800A2210[];
extern s32 D_8009F7B8;

void func_8004F0F0(s32 index, u32 mode, s32 entry)
{
    u8 *flag;
    u8 value;
    struct Unk4F0F0Entry *table;

    if (mode < 0x100 && index >= 0 && index < D_8009F7B8) {
        table = D_800A2210[index];
        if (mode == 0) {
            goto clear;
        }
        if (mode == 1) {
            goto set;
        }
        goto done;
clear:
        flag = &table[entry].flags;
        value = *flag & ~2;
        goto store;
set:
        flag = &table[entry].flags;
        value = *flag | 2;
store:
        *flag = value;
    }
done:
    return;
}
