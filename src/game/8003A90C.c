#include "game/types.h"

struct Unk3A90CEntry {
    u8 unk00[3];
    u8 start;
    u8 count;
    u8 unk05[2];
    u8 flags;
    u8 unk08[0x0C];
};

struct Unk3A90CEntry *func_8003A90C(struct Unk3A90CEntry *entry, s32 key)
{
    struct Unk3A90CEntry *result;
    s32 delta;

    result = 0;
    while ((delta = key - entry->start) < 0 || delta >= entry->count) {
        if (entry->flags & 0x80) {
            goto done;
        }
        entry++;
    }
    result = entry;
done:
    return result;
}
