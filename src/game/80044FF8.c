#include "../../include/game/types.h"

struct ModeObject {
    u8 pad_00[8];
    s32 value;
};

struct ModeState {
    s32 mode;
    struct ModeObject *object;
    s32 value;
};

extern struct ModeState D_800A1800;

void func_80044FF8(s32 mode, struct ModeObject *object)
{
    D_800A1800.mode = mode;
    if (mode != -1) {
        D_800A1800.object = object;
        D_800A1800.value = object->value;
    }
}
