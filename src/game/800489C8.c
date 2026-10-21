struct LookupState {
    unsigned char pad_00[0x10];
    unsigned char *values;
};

unsigned char *func_800489C8(struct LookupState *state, unsigned int offset)
{
    unsigned char index = *((unsigned char *)state + offset + 0x1C);
    return state->values + index * 11;
}
