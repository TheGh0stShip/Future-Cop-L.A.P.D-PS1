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
    s32 value;
    u16 half;

    if (anim != 0) {
        if (owner->animB8 != anim) {
            owner->mirror->anim90 = anim;
            owner->mirror->anim68 = anim;
            owner->animB8 = anim;
            value = anim->value08;
            owner->mirror->value94 = value;
            owner->mirror->value6C = value;
            owner->valueBC = value;
        }
    } else {
        half = owner->valueB4;
        owner->mirror->value8E = half;
        owner->mirror->value66 = half;
        owner->valueB6 = half;
        if (owner->modeA6 != 4) {
            owner->mirror->mode7E = 4;
            owner->mirror->mode56 = 4;
            owner->modeA6 = 4;
            half = owner->mirror->base1B;
            owner->mirror->value80 = half;
            owner->mirror->value58 = half;
            owner->valueA8 = half;
        }
    }
}
