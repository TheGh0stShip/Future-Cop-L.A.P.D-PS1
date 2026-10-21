typedef unsigned short u16;

struct ValuePair {
    unsigned char pad[0x10C];
    u16 source;
    u16 destination;
};

void func_8002B1D0(struct ValuePair *value)
{
    value->destination = value->source;
}
