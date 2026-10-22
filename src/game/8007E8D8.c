#include "game/types.h"

void func_8007E8D8(u8 *output, u32 value1, u32 value2, u32 value3,
                   u32 value4, u32 value5, u32 value6, u32 value7,
                   u32 value8, u32 value9, u32 value10, u32 value11,
                   u32 value12, u32 value13)
{
    register u8 *destination __asm__("$24");
    register u32 packed4 __asm__("$4");
    register u32 packed5 __asm__("$14");
    register u32 packed6 __asm__("$25");
    register u32 group0 __asm__("$17");
    register u32 savedValue3 __asm__("$18");
    register u32 savedValue7 __asm__("$16");
    register u32 packed8 __asm__("$12");
    register u32 packed9 __asm__("$11");
    register u32 packed10 __asm__("$9");
    register u32 packed11 __asm__("$10");
    register u32 packed12 __asm__("$13");
    register u32 packed13 __asm__("$8");
    register u32 upperGroup __asm__("$19");
    u32 group1;

    destination = output;
    savedValue3 = value3;
    packed4 = value4;
    packed5 = value5;
    packed6 = value6;
    savedValue7 = value7;
    packed8 = value8;
    packed9 = value9;
    packed10 = value10;
    packed11 = value11;
    packed12 = value12;
    packed13 = value13;
    group0 = packed5 + packed6 + savedValue7 + packed8 + packed9 + packed12;
    group1 = value1 + value2 + savedValue3 + packed4 + packed5 + packed6 +
             savedValue7 + packed8 + packed9 + packed10 + packed11 + packed12 +
             packed13;

    upperGroup = (group1 & 0xF) >> 2;

    if ((u8)packed6 == 10) {
        if ((u8)savedValue7 == (u8)packed6) {
            if ((u8)packed8 == (u8)savedValue7 && (u8)packed9 == (u8)packed8) {
                u32 first;
                u32 second;

                if ((u8)packed12 < 6) {
                    first = (u8)packed12;
                    second = 0;
                } else {
                    first = 5;
                    second = (u8)packed12 - 5;
                }
                packed6 += first;
                savedValue7 += second;
            }
        }
    }

    destination[0] = (group0 & 3) | ((packed9 & 3) << 2);
    destination[1] = (packed5 & 3) | (value1 << 2);
    destination[2] = (group1 & 3) | ((packed6 & 3) << 2);
    destination[3] = ((packed5 >> 2) & 3) | (packed4 << 2);
    destination[5] = value2 | ((packed9 >> 1) & 6);
    destination[7] = packed10 | (packed11 << 1) | (savedValue3 << 2);
    destination[8] = ((packed6 >> 2) & 3) | (upperGroup << 2);
    destination[4] = savedValue7;
    destination[6] = packed8;
    destination[9] = packed13 | ((group1 & 3) << 1);
}
