#include "game/types.h"

struct Unk56994 {
    u8 unk00[0xC1];
    s8 side;
    u8 unkC2[0x23C];
    s16 timer;
    u8 unk300[0x13];
    u8 step;
};

extern u8 D_800A2290[][8];

u8 func_80056994(struct Unk56994 *state, s32 elapsed)
{
    state->timer -= elapsed;
    if (state->timer < 0) {
        state->timer = 3000;
        state->step++;
    }
    return D_800A2290[state->side][state->step];
}
