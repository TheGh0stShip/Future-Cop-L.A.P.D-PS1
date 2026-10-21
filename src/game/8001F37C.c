struct GlobalState {
    unsigned char pad[0x9A4];
    int value;
};

extern struct GlobalState *D_8009AB08;

int func_8001F37C(void)
{
    return D_8009AB08->value;
}
