#include "game/types.h"

struct Unk516A0Owner {
    u8 unk00[0x2C];
    s32 transform[15];
    u8 unk68[0x44];
    u8 *state;
};

extern void func_80013BD8(void *list, void *entry, s32 mode);

void func_800516A0(struct Unk516A0Owner *owner)
{
    register u8 *state __asm__("$3");
    register u8 *entry __asm__("$13");
    register u8 *clearCursor __asm__("$14");
    register u32 mask16_19 __asm__("$10");
    register u32 mask20_23 __asm__("$12");
    register u32 mask24 __asm__("$11");
    register u32 mask25 __asm__("$8");
    register u32 mask26 __asm__("$6");
    register u32 mask28 __asm__("$9");
    register u32 mask29 __asm__("$7");
    register u32 mask30 __asm__("$5");
    register u32 mask31 __asm__("$4");
    u32 flags;
    register s32 i __asm__("$15");

    mask16_19 = 0xFFF0FFFF;
    mask20_23 = 0xFF0FFFFF;
    mask24 = 0xFEFFFFFF;
    mask25 = 0xFDFFFFFF;
    mask26 = 0xFBFFFFFF;
    mask28 = 0xEFFFFFFF;
    mask29 = 0xDFFFFFFF;
    mask30 = 0xBFFFFFFF;
    mask31 = 0x7FFFFFFF;

    state = owner->state;
    entry = state + 0x2A8;
    clearCursor = entry + 3;

    *(s32 *)(entry + 0x00) = 0;
    *(u16 *)(entry + 0x0C) = 1;
    flags = *(u32 *)(entry + 0x0C);
    *(s32 *)(entry + 0x04) = 0;
    *(s32 *)(entry + 0x08) = 0;
    *(s32 *)(entry + 0x10) = 0;
    *(s32 *)(entry + 0x14) = 0;
    *(s32 *)(entry + 0x18) = 0;
    *(s32 *)(entry + 0x1C) = 0;
    *(s32 *)(entry + 0x20) = 0;
    *(s32 *)(entry + 0x24) = 0;
    *(s32 *)(entry + 0x28) = 0;
    *(s32 *)(entry + 0x2C) = 0;
    *(s32 *)(entry + 0x38) = 0;

    flags &= mask16_19;
    flags |= 0x00010000;
    flags &= mask20_23;
    flags &= mask24;
    flags &= mask25;
    flags &= mask26;
    flags |= 0x08000000;
    flags &= mask28;
    flags &= mask29;
    flags &= mask30;
    flags &= mask31;
    *(u32 *)(entry + 0x0C) = flags;

    for (i = 3; i >= 0; i--) {
        clearCursor[0x30] = 0;
        clearCursor--;
    }

    *(s32 *)(entry + 0x3C) = 1;
    *(u8 **)(entry + 0x34) = entry + 0x3C;
    *(s32 *)(entry + 0x40) = 0x10;
    func_80013BD8(state + 0x25C, entry, 0);

    *(s32 *)(owner->state + 0x260) = owner->transform[0];
    *(s32 *)(owner->state + 0x264) = owner->transform[1];
    *(s32 *)(owner->state + 0x268) = owner->transform[2];
    *(s32 *)(owner->state + 0x26C) = owner->transform[3];
    *(s32 *)(owner->state + 0x270) = owner->transform[4];
    *(s32 *)(owner->state + 0x274) = owner->transform[5];
    *(s32 *)(owner->state + 0x278) = owner->transform[6];
    *(s32 *)(owner->state + 0x27C) = owner->transform[7];
    *(s32 *)(owner->state + 0x280) = owner->transform[8];
    *(s32 *)(owner->state + 0x284) = owner->transform[9];
    *(s32 *)(owner->state + 0x288) = owner->transform[10];
    *(s32 *)(owner->state + 0x28C) = owner->transform[11];
    *(s32 *)(owner->state + 0x290) = owner->transform[12];
    *(s32 *)(owner->state + 0x294) = owner->transform[13];
    *(s32 *)(owner->state + 0x298) = owner->transform[14];
}
