#include "game/types.h"

struct Unk42504Def {
    u8 unk00[0x24];
    u8 value24;
    u8 unk25;
    u8 value26;
    u8 value27;
    s32 value28;
    u8 bytes[8];
    s32 words[8];
};

struct Unk42504Spawn {
    u8 unk00[8];
    struct Unk42504Def *def;
};

struct Unk42504Object {
    u8 unk00[0x24];
    u32 flags;
    u8 unk28[0x78];
    u8 valueA0;
    u8 activeA1;
    u8 valueA2;
    u8 valueA3;
    s32 valueA4;
    u8 bytes[8];
    s32 words[8];
};

extern void func_8002AD58(s32 kind);

s32 func_80042504(s32 message, struct Unk42504Object *object, struct Unk42504Spawn *spawn)
{
    struct Unk42504Def *def;
    s32 i;

    func_8002AD58(2);
    object->flags |= 0x20000D;
    def = spawn->def;
    object->valueA0 = def->value24;
    object->activeA1 = 1;
    object->valueA2 = def->value26;
    object->valueA3 = def->value27;
    object->valueA4 = def->value28;
    for (i = 0; i < 8; i++) {
        object->bytes[i] = def->bytes[i];
        object->words[i] = def->words[i];
    }
    return 0;
}
