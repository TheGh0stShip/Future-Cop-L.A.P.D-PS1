#include "game/types.h"

struct Unk6158CDef {
    s32 id;
};

struct Unk6158CInfo {
    u8 unk00[0x28];
    struct Unk6158CDef *def;
};

struct Unk6158CEntry {
    s32 unk00;
    struct Unk6158CInfo *info;
};

struct Unk60E28Table {
    s32 count;
    struct Unk6158CEntry *entries[0x80];
};

extern struct Unk60E28Table *D_8009F800;
extern void func_800614BC(struct Unk6158CEntry *entry);

void func_8006158C(s32 id)
{
    struct Unk60E28Table *table;
    struct Unk6158CEntry **cursor;
    struct Unk6158CEntry *entry;
    s32 remaining;

    table = D_8009F800;
    cursor = table->entries;
    for (remaining = table->count; remaining > 0; remaining--) {
        entry = *cursor++;
        if (entry->info->def->id == id) {
            func_800614BC(entry);
            return;
        }
    }
}
