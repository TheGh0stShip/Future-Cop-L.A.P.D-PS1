#include "game/types.h"

struct Unk7938 {
    u8 unk00[0x10];
    u8 active;
    u8 unk11;
    u8 unk12;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 unk16;
    u8 unk17;
    s32 unk18;
};

extern struct Unk7938 D_800A7938[];

void func_80045504(s32 index)
{
    struct Unk7938 *entry;

    entry = &D_800A7938[index];
    entry->unk13 = 1;
    entry->active = 0;
    entry->unk15 = 0;
    entry->unk11 = 0;
    entry->unk12 = 0;
    entry->unk14 = 0;
    entry->unk16 = 0xFF;
    entry->unk18 = 0;
}
