#include "game/types.h"

struct InputMaskMap {
    u32 masks[14];
};

extern struct InputMaskMap D_800AAC50[2];
extern u32 func_8001EF8C(void);

u32 func_8001E620(u32 profile)
{
    register struct InputMaskMap *map __asm__("$5");
    register u32 pressed __asm__("$6");
    register u32 result __asm__("$4");
    register u32 offset __asm__("$3");
    register u32 returnValue __asm__("$2");
    register u32 firstMask __asm__("$2");
    register u32 secondMask __asm__("$3");
    register u32 firstSet __asm__("$2");

    pressed = func_8001EF8C();
    result = 0;
    if (profile < 2) {
        offset = (profile * 8) - profile;
        offset <<= 3;
        map = (struct InputMaskMap *)((u8 *)D_800AAC50 + offset);
        firstMask = map->masks[0];
        secondMask = map->masks[1];
        firstSet = (pressed & firstMask) != 0;
        result = firstSet << 30;
        if (pressed & secondMask) {
            result |= 0x80000000;
        }
        if (pressed & map->masks[2]) {
            result |= 0x10000000;
        }
        if (pressed & map->masks[3]) {
            result |= 0x20000000;
        }
        if (pressed & map->masks[4]) {
            result |= 0x08000000;
        }
        if (pressed & map->masks[6]) {
            result |= 0x02000000;
        }
        if (pressed & map->masks[5]) {
            result |= 0x04000000;
        }
        if (pressed & map->masks[7]) {
            result |= 0x01000000;
        }
        if (pressed & map->masks[13]) {
            result |= 0x00080000;
        }
        if (pressed & map->masks[12]) {
            result |= 0x00040000;
        }
        if (pressed & map->masks[8]) {
            result |= 0x00800000;
        }
        if (pressed & map->masks[9]) {
            result |= 0x00400000;
        }
        if (pressed & map->masks[11]) {
            result |= 0x00200000;
        }
        firstSet = pressed & map->masks[10];
        if (firstSet) {
            result |= 0x00100000;
        }
    }
    returnValue = result;
    return returnValue;
}
