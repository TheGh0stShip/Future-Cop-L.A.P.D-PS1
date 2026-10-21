struct CommandState {
    unsigned char pad_00[0x24];
    unsigned char data;
    unsigned char pad_25[7];
    unsigned char *value;
    unsigned char pad_30[5];
    unsigned char active;
    unsigned char command;
};

void func_8008B71C(struct CommandState *state, unsigned char value)
{
    state->command = 0x43;
    state->value = &state->data;
    state->data = value;
    state->active = 1;
}
