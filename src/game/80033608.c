#include "game/types.h"

struct Unk33608Anim {
    u8 unk00[8];
    s32 value08;
};

struct Unk33608Mirror {
    u8 unk00[0x1B];
    u8 base1B;
    u8 unk1C[0x3A];
    u8 mode56;
    u8 unk57;
    u16 value58;
    u8 unk5A[0x0C];
    u16 value66;
    struct Unk33608Anim *anim68;
    s32 value6C;
    u8 unk70[0x0E];
    u8 mode7E;
    u8 unk7F;
    u16 value80;
    u8 unk82[0x0C];
    u16 value8E;
    struct Unk33608Anim *anim90;
    s32 value94;
};

struct Unk33608Owner {
    u8 unk00[0xA6];
    u8 modeA6;
    u8 unkA7;
    u16 valueA8;
    u8 unkAA[0x0A];
    u16 valueB4;
    u16 valueB6;
    struct Unk33608Anim *animB8;
    s32 valueBC;
    u8 unkC0[0x74];
    struct Unk33608Mirror *mirror;
};

void func_80033608(struct Unk33608Owner *owner, struct Unk33608Anim *anim)
{
    register struct Unk33608Mirror *firstMirror __asm__("$2");
    register struct Unk33608Mirror *secondMirror __asm__("$3");
    register s32 value __asm__("$2");
    register u16 half __asm__("$3");
    register u16 lastHalf __asm__("$2");

    if (anim != 0) {
        if (owner->animB8 != anim) {
            firstMirror = owner->mirror;
            firstMirror->anim90 = anim;
            firstMirror->anim68 = anim;
            secondMirror = owner->mirror;
            owner->animB8 = anim;
            value = anim->value08;
            secondMirror->value94 = value;
            secondMirror->value6C = value;
            owner->valueBC = value;
        }
    } else {
        firstMirror = owner->mirror;
        half = owner->valueB4;
        firstMirror->value8E = half;
        firstMirror->value66 = half;
        owner->valueB6 = half;
        if (owner->modeA6 != 4) {
            firstMirror = owner->mirror;
            firstMirror->mode7E = 4;
            firstMirror->mode56 = 4;
            owner->modeA6 = 4;
            secondMirror = owner->mirror;
            lastHalf = secondMirror->base1B;
            secondMirror->value80 = lastHalf;
            secondMirror->value58 = lastHalf;
            owner->valueA8 = lastHalf;
        }
    }
}
