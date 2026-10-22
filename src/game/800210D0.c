#include "game/types.h"

extern u16 D_8009AB0C[];
extern u16 D_8009AB18[];

u16 func_800210D0(u32 character)
{
    union {
        u16 half;
        u8 bytes[2];
    } converted;
    register u32 page __asm__("$5");
    register u32 base __asm__("$3");
    register u32 original __asm__("$6");
    u32 value;

    page = 0;
    base = 0;
    original = character;

    if ((u8)(character - 0x20) < 0x10) {
        base = 1;
        goto convert;
    }
    if ((u8)(character - 0x30) < 0xA) {
        goto convert;
    }
    if ((u8)(character - 0x3A) < 7) {
        base = 0xB;
        goto convert;
    }
    if ((u8)(character - 0x41) < 0x1A) {
        page = 1;
        goto convert;
    }
    if ((u8)(character - 0x5B) < 6) {
        base = 0x25;
        goto convert;
    }
    if ((u8)(character - 0x61) < 0x1A) {
        page = 2;
        goto convert;
    }
    if ((u8)(character - 0x7B) < 4) {
        base = 0x3F;
        goto convert;
    }
    return 0;

convert:
    if ((u8)base != 0) {
        value = D_8009AB18[(u8)original - ((u8)base + 0x1F)];
    } else {
        u16 *range;

        range = &D_8009AB0C[page * 2];
        value = range[0] + (u8)original - range[1];
    }
    converted.half = value;
    return ((u16)converted.bytes[0] << 8) | converted.bytes[1];
}
