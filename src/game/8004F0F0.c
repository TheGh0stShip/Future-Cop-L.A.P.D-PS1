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
    struct Unk4F0F0Entry *table;

    if (mode < 0x100 && index >= 0 && index < D_8009F7B8) {
        table = D_800A2210[index];
        if (mode == 0) {
            table[entry].flags &= ~2;
        } else if (mode == 1) {
            table[entry].flags |= 2;
        }
    }
}
