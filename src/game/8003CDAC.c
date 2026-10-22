#include "game/types.h"

struct Unk3CDACRange {
    u32 count;
};

struct Unk3CDACObject {
    u8 unk00[0x0C];
    u32 flags;
    u8 unk10[0x20];
    u8 slots[8];
    struct Unk3CDACRange *range;
};

struct Unk3CDACHolder {
    struct Unk3CDACObject *object;
};

s32 func_8003CDAC(struct Unk3CDACHolder *holder, s8 index)
{
    struct Unk3CDACObject *object;

    object = holder->object;
    if (object == 0) {
        return 0;
    }
    if ((object->flags >> 25) & 1) {
        if (object->range == 0 || index < 0) {
            return 0;
        }
        return index < object->range->count;
    }
    return object->slots[index] != 0xFF;
}
