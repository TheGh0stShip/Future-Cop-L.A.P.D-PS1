typedef unsigned short u16;

struct FlagValue {
    unsigned char pad[0xE];
    u16 flags;
};

void func_80073BA8(struct FlagValue *value)
{
    value->flags |= 0x1000;
}
