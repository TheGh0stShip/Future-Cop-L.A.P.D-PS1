#include "game/types.h"

struct Unk45164Slot {
    u8 unk00[8];
    s32 key;
};

struct Unk45164Current {
    s32 unk00;
    struct Unk45164Slot *slot;
    s32 key;
};

extern struct Unk45164Current D_800A1800;

extern u8 func_80045028(s32 id);

struct Unk45164Slot *func_80045164(s32 id)
{
    register s32 valid __asm__("$3");
    register struct Unk45164Slot *result __asm__("$2");
    struct Unk45164Current *current;

    valid = func_80045028(id);
    result = 0;
    if (valid != 0) {
        current = &D_800A1800;
        if (current->slot->key == current->key) {
            result = current->slot;
        }
    }
    return result;
}
