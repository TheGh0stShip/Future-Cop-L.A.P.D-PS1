#include "game/types.h"

struct Unk57358Info {
    u8 unk00[0xD0];
    s16 charge;
};

struct Unk57358 {
    u8 unk00[0xAC];
    struct Unk57358Info *info;
};

void func_80057358(struct Unk57358 *object, s32 amount)
{
    struct Unk57358Info *info;

    info = object->info;
    if (info->charge < 0x100) {
        amount /= 4;
        if (amount >= 0x100) {
            amount = 0xFF;
        }
        info->charge += amount;
    }
}
