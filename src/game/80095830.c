void func_80095830(unsigned char *destination, int value, unsigned int count)
{
    while (count-- != 0) {
        *destination++ = value;
    }
}
