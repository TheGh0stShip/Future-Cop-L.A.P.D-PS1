#include "game/types.h"

struct Unk5C048Def {
    u8 unk00[4];
    u8 kind;
    u8 unk05[0x29];
    u16 value;
};

struct Unk5C048Result {
    u8 unk00[0x20];
    struct Unk5C048Def *def;
};

struct Unk5C048Event {
    u16 value;
    u8 unk02[0x0D];
    u8 column;
};

extern s32 func_8003BF50(void *object);
extern struct Unk5C048Result *func_800402DC(void *owner, u8 column, s32 arg2, s32 row,
                                            s32 arg4);

void func_8005C048(void *object, struct Unk5C048Event *event)
{
    struct Unk5C048Result *result;

    result = func_800402DC(object, event->column, 0, func_8003BF50(object), 0);
    if (result != 0 && result->def->kind == 0x5C) {
        result->def->value = event->value;
    }
}
