#include "game/types.h"

struct Unk631A0 {
    s32 key;
    u8 value;
};

extern s32 D_8009F838;
extern struct Unk631A0 D_800A31A0[];

s32 func_8006314C(s32 key, u8 value)
{
    struct Unk631A0 *entry;

    if (D_8009F838 == 0x18) {
        return 0;
    }
    entry = &D_800A31A0[D_8009F838];
    entry->value = value;
    entry->key = key;
    D_8009F838++;
    return 1;
}
