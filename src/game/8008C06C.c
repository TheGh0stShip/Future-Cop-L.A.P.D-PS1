struct CommandState {
    unsigned char pad_00[0x36];
    unsigned char current;
    unsigned char previous;
};

int func_8008C06C(struct CommandState *state)
{
    int current = state->current;
    state->current = 0;
    state->previous = current;
    return current;
}
