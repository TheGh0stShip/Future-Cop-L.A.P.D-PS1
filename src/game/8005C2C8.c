#include "game/types.h"

struct Unk5C2C8 {
    u8 unk00[0x2A];
    u8 count;
    u8 unk2B[0x0D];
    s32 items[10];
};

s32 func_8005C2C8(struct Unk5C2C8 *set, s32 value)
{
    s32 i;

    if (set->count >= 0xA) {
        return 1;
    }
    for (i = 0; i < set->count; i++) {
        if (set->items[i] == value) {
            return 1;
        }
    }
    set->items[set->count] = value;
    set->count++;
    return 0;
}
