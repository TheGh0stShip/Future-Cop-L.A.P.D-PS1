#include "../../include/game/types.h"

struct PacketState {
    s16 x;
    s16 y;
    s16 width;
    s16 height;
    s16 values[4];
    u8 flags[4];
};

struct PacketState *func_800921F4(struct PacketState *state, s16 x, s16 y,
                                  s16 width, s16 height)
{
    state->x = x;
    state->y = y;
    state->width = width;
    state->height = height;
    state->values[0] = 0;
    state->values[1] = 0;
    state->values[2] = 0;
    state->values[3] = 0;
    state->flags[0] = state->flags[1] = 0;
    state->flags[2] = state->flags[3] = 0;
    return state;
}
