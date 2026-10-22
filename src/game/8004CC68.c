#include "game/types.h"

struct Unk4CC68 {
    u8 unk00[0x44];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
};

extern s32 D_8009FAA4;
extern struct Unk4CC68 D_800A6F60;

void func_8004CC68(void)
{
    s32 level;

    level = D_8009FAA4 >> 3;
    level &= 0x3F;
    if (level >= 0x20) {
        level = 0x3F - level;
    }
    level += 8;
    if (level >= 0x20) {
        level = 0x1F;
    }
    D_800A6F60.unk44 = level << 3;
    D_800A6F60.unk48 = ((level / 2) << 3) | ((level / 2) << 11) | (level << 19);
    D_800A6F60.unk4C = level << 11;
}
