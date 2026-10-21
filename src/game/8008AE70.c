#include "../../include/game/command_state.h"

void func_8008AE70(struct CommandState *state, int value, u8 type)
{
    *(int *)&state->inline_data[4] = value;
    state->type = type;
}
