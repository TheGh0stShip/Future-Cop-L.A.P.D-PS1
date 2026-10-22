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
    if (func_80045028(id) == 0) {
        return 0;
    }
    if (D_800A1800.slot->key != D_800A1800.key) {
        return 0;
    }
    return D_800A1800.slot;
}
