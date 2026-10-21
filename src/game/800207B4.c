struct GlobalState {
    unsigned char pad[0x900];
    int value;
};

extern struct GlobalState *D_8009AB08;

int func_800207B4(void)
{
    return D_8009AB08->value;
}
