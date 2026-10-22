#include "game/types.h"

struct Unk3BObject {
    u8 unk00[0x1C];
    s16 unk1C;
    s16 unk1E;
    u8 tableIndex;
    u8 unk21[3];
    u32 flags;
};

extern u8 *D_800A0C70[];
extern s32 func_8003B65C(s32 value, s32 range);

void func_8003BA50(struct Unk3BObject *object, s32 value)
{
    s32 previous;
    s32 current;

    if (object->unk1C >= 0) {
        previous = func_8003B65C(value, object->unk1E);
        current = func_8003B65C(object->unk1C, object->unk1E);
        if (D_800A0C70[object->tableIndex] != 0 && current != previous) {
            object->flags |= 0x4000;
        }
    }
}
