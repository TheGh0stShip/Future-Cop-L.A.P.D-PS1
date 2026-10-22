#include "../../include/game/types.h"

struct ModeState {
    s32 mode;
    void *object;
    s32 value;
};

extern struct ModeState D_800A1800;

s32 func_80045028(s32 mode)
{
    if (D_800A1800.mode == -1) {
        return 0;
    }
    if (mode == D_800A1800.mode) {
        return 1;
    }
    return 0;
}
