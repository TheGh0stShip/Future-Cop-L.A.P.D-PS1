#include "game/types.h"

u8 *func_8001E44C(u8 *destination, u8 *source, s32 count)
{
    u8 *to;
    u8 *from;

    to = destination;
    if (source < destination && destination < source + count) {
        to = destination + count;
        from = source + count;
        while (count != 0) {
            *--to = *--from;
            count--;
        }
        return destination;
    }
    while (count != 0) {
        *to++ = *source++;
        count--;
    }
    return destination;
}
