struct CommandFields {
    unsigned char pad_00[0x28];
    int value;
    unsigned char pad_2C[8];
    unsigned char type;
};

void func_8008AE70(struct CommandFields *fields, int value,
                   unsigned char type)
{
    fields->value = value;
    fields->type = type;
}
