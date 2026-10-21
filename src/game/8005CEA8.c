#include "../../include/game/types.h"

struct MotionState {
    s32 x;
    s32 y;
    s32 scale;
    s16 angle;
    u8 pad_0E[2];
    s16 velocity;
    u8 pad_12[2];
    s32 timer;
    u8 enabled;
    u8 state;
};

void func_8005CEA8(struct MotionState *state)
{
    state->enabled = 1;
    state->timer = 0;
    state->angle = 0;
    state->velocity = 0;
    state->state = 0;
    state->x = 0;
    state->y = 0;
    state->scale = 0x10000;
}
