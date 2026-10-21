#include "../../include/game/command_state.h"

void func_8008B4EC(struct CommandState *state)
{
    state->current = 0x4D;
    state->active = 6;
    state->value = state->source;
}
