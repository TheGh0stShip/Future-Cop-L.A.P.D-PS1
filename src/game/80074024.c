#include "game/types.h"

struct Unk74024Def {
    u8 unk00[0x36];
    u16 width;
    u16 height;
    u8 unk3A[0x10];
    u16 flags;
};

struct Unk74024 {
    u8 unk00[8];
    s16 unk08;
    s16 unk0A;
    u8 unk0C[0x18];
    void (*update)();
    u8 unk28[0x3C];
    s32 unk64;
};

extern void func_80073BBC();

extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8003C14C(struct Unk74024 *object, struct Unk74024Def *def);

struct Unk74024 *func_80074024(struct Unk74024Def *def)
{
    struct Unk74024 *object;
    s32 size;

    object = func_80017EBC(0x68, 1);
    if (object == 0) {
        return 0;
    }
    func_8003C14C(object, def);
    object->update = func_80073BBC;
    object->unk64 = 0;
    size = def->height * def->width;
    def->flags |= 0x10;
    object->unk08 = 0;
    object->unk0A = size * 2;
    return object;
}
