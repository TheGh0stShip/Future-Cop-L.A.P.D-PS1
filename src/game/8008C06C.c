#include "../../include/game/command_state.h"

int func_8008C06C(struct CommandState *state)
{
    int current = state->current;
    state->current = 0;
    state->previous = current;
    return current;
}
