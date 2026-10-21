int func_800489EC(unsigned char *value)
{
    int result = 4;

    if (*value == 0x40) {
        result = 4;
    } else if (*value == 0x44) {
        result = 1;
    }
    return result;
}
