#include "game/types.h"

void func_8007E8D8(u8 *output, u32 value1, u32 value2, u32 value3,
                   u32 value4, u32 value5, u32 value6, u32 value7,
                   u32 value8, u32 value9, u32 value10, u32 value11,
                   u32 value12, u32 value13)
{
    register u32 group0 __asm__("$17");
    register u32 savedValue3 __asm__("$18");
    register u32 savedValue7 __asm__("$16");
    register u32 upperGroup __asm__("$19");
    u32 group1;

    savedValue3 = value3;
    savedValue7 = value7;
    group0 = value5 + value6 + value7 + value8 + value9 + value12;
    group1 = value1 + value2 + value3 + value4 + value5 + value6 + value7 +
             value8 + value9 + value10 + value11 + value12 + value13;

    upperGroup = (group1 & 0xF) >> 2;

    if ((u8)value6 == 10) {
        if ((u8)savedValue7 == (u8)value6) {
            if ((u8)value8 == (u8)savedValue7 && (u8)value9 == (u8)value8) {
                u32 first;
                u32 second;

                if ((u8)value12 < 6) {
                    first = (u8)value12;
                    second = 0;
                } else {
                    first = 5;
                    second = (u8)value12 - 5;
                }
                value6 += first;
                savedValue7 += second;
            }
        }
    }

    output[0] = (group0 & 3) | ((value9 & 3) << 2);
    output[1] = (value5 & 3) | (value1 << 2);
    output[2] = (group1 & 3) | ((value6 & 3) << 2);
    output[3] = ((value5 >> 2) & 3) | (value4 << 2);
    output[5] = value2 | ((value9 >> 1) & 6);
    output[7] = value10 | (value11 << 1) | (savedValue3 << 2);
    output[8] = ((value6 >> 2) & 3) | (upperGroup << 2);
    output[4] = savedValue7;
    output[6] = value8;
    output[9] = value13 | ((group1 & 3) << 1);
}
