struct CommandState {
    unsigned char pad_00[0x20];
    int *source;
    unsigned char pad_24[8];
    int *value;
    unsigned char pad_30[5];
    unsigned char active;
    unsigned char command;
};

void func_8008B4EC(struct CommandState *state)
{
    state->command = 0x4D;
    state->active = 6;
    state->value = state->source;
}
