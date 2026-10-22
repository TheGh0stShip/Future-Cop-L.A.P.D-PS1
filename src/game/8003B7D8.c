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

u8 func_8003B7D8(struct Unk3BObject *object)
{
    u8 *entry;

    if (object->tableIndex == 0) {
        return 0;
    }
    if (object->flags & 0x40000) {
        return 0;
    }
    entry = D_800A0C70[object->tableIndex];
    if (entry == 0) {
        return 0;
    }
    return entry[1];
}
