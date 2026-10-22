#include "game/types.h"

struct Unk3BObject {
    u8 unk00[0x1C];
    s16 unk1C;
    s16 unk1E;
    u8 tableIndex;
    u8 unk21[3];
    u32 flags;
};

extern u8 func_8003B838(struct Unk3BObject *object);
extern s32 func_8003BF50(struct Unk3BObject *object);
extern void func_800402DC(struct Unk3BObject *object, u8 kind, s32 arg2,
                          s32 arg3, s32 arg4);

void func_8003BC0C(struct Unk3BObject *object)
{
    u8 kind;

    if (object->tableIndex != 0) {
        kind = func_8003B838(object);
        if (kind != 0) {
            func_800402DC(object, kind, 0, func_8003BF50(object), 0);
        }
    }
}
