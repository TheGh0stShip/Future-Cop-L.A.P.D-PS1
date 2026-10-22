#include "game/types.h"

struct Unk41DC4 {
    u8 unk00[0x16];
    u8 kind;
    u8 unk17[0x0D];
    u32 flags;
    u8 unk28[0x78];
    u8 stateA0;
    u8 unkA1[3];
    s16 valueA4;
};

extern s32 D_8009F9B0;

void func_80041DC4(struct Unk41DC4 *object, s16 value)
{
    if (object->kind == 0x22) {
        object->valueA4 = value;
        D_8009F9B0++;
        object->flags |= 8;
        object->stateA0 |= 1;
    }
}
