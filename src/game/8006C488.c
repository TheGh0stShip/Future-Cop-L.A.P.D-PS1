#include "game/types.h"

struct Unk6C488Stats {
    u8 unk00[0x8A];
    s16 unk8A;
};

struct Unk6C488 {
    u8 unk00[0xAC];
    struct Unk6C488Stats *stats;
};

extern void func_80046CF0(struct Unk6C488 *object, s32 state);

void func_8006C488(struct Unk6C488 *object)
{
    s32 state;

    state = 0x32;
    if (object->stats->unk8A > 0) {
        state = 0x33;
    }
    func_80046CF0(object, state);
}
