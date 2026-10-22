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
    register struct Unk3CDACRange *range __asm__("$3");
    register u32 flagWord __asm__("$2");
    register s32 result __asm__("$2");

    object = holder->object;
    result = 0;
    if (object == 0) {
        goto done;
    }
    flagWord = object->flags;
    flagWord >>= 25;
    flagWord &= 1;
    if (flagWord != 0) {
        range = object->range;
        if (range == 0) {
            goto zero;
        }
        if (index < 0) {
            goto done;
        }
        if (index < range->count) {
            result = 1;
            goto done;
        }
zero:
        result = 0;
        goto done;
    }
    if (object->slots[index] == 0xFF) {
        goto done;
    }
    result = 1;
done:
    return result;
}
