#include "game/types.h"

u32 func_80018818(u32 divisor)
{
    u32 remainder;
    u32 quotient;
    u32 carry;
    s32 i;

    remainder = 2;
    quotient = 0;
    carry = quotient;
    for (i = 32; i > 0; i--) {
        quotient <<= 1;
        if (carry != 0 || divisor < remainder) {
            remainder -= divisor;
            quotient |= 1;
        }
        carry = remainder & 0x80000000;
        remainder <<= 1;
    }
    return quotient;
}
