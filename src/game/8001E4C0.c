void *func_8001E4C0(void *destination, int value, unsigned int count)
{
    unsigned char *current = destination;
    void *result = destination;

    while (count != 0) {
        *current++ = value;
        count--;
    }
    return result;
}
