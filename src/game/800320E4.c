#include "game/types.h"

struct Unk320E4 {
    u8 unk00[0x16];
    u8 kind;
    u8 unk17[0xB9];
    s32 baseX;
    s32 baseY;
    s32 baseZ;
    s32 targetX;
    s32 targetY;
    s32 targetZ;
    u8 unkE8[0x12];
    u8 state;
    u8 unkFB[0x11];
    s16 spreadZ;
    s16 spreadX;
};

extern s32 func_80019668(void);

void func_800320E4(struct Unk320E4 *object)
{
    if (object->state != 3 && object->kind != 0x25) {
        object->targetY = object->baseY;
        object->targetX = object->baseX +
            ((((func_80019668() & 0x7FF) - 0x400) * object->spreadX) >> 3);
        object->targetZ = object->baseZ +
            ((((func_80019668() & 0x7FF) - 0x400) * object->spreadZ) >> 3);
    }
}
