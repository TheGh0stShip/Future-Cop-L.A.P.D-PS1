struct CommandState {
    unsigned char pad_00[0x2C];
    int value;
    unsigned char pad_30[5];
    unsigned char active;
    unsigned char command;
};

void func_8008B73C(struct CommandState *state)
{
    state->command = 0x45;
    state->value = 0;
    state->active = 0;
}
