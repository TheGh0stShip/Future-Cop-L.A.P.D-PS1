#include "game/types.h"

struct Unk53730Info {
    u8 unk00[0xC1];
    s8 side;
};

struct Unk53730 {
    u8 unk00[0x50];
    s32 x;
    s32 y;
    s32 z;
    u8 unk5C[0x50];
    struct Unk53730Info *info;
};

struct Unk53730Global {
    u8 unk00[0x5C];
    s32 unk5C;
};

struct Unk53730Delta {
    s32 x;
    s32 z;
};

extern s32 D_8009FA0C[];
extern struct Unk53730Global *D_8009FA94[];
extern struct Unk53730 *D_8009FA58[];
extern s32 D_8009FA64;

extern s32 func_8001A074(struct Unk53730Delta *delta);

void func_80053730(struct Unk53730 *object)
{
    struct Unk53730 *other;
    struct Unk53730Delta delta;

    if (D_8009FA0C[0] == 2 && D_8009FA94[0]->unk5C == 0) {
        other = D_8009FA58[1 - object->info->side];
        delta.x = other->x - object->x;
        delta.z = other->z - object->z;
        D_8009FA64 = func_8001A074(&delta);
    }
}
