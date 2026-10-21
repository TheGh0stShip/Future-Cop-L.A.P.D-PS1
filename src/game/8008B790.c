#include "../../include/game/command_state.h"

void func_8008B790(struct CommandState *state, u8 value)
{
    state->current = 0x47;
    state->value = state->inline_data;
    state->inline_data[0] = value;
    state->active = 1;
}
