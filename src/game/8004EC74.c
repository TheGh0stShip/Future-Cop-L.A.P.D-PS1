#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
    u8 unk0C[0x7C];
    s32 unk88;
    u8 unk8C[4];
    s16 unk90;
};

struct EntityTable {
    s32 count;
    s32 unk04;
    struct Entity *items[1];
};

extern struct EntityTable D_800A8F90;

void func_8004EC74(void)
{
    struct Entity **cursor;
    struct Entity *entity;
    s32 remaining;

    cursor = D_800A8F90.items;
    for (remaining = D_800A8F90.count; remaining > 0; remaining--) {
        entity = *cursor++;
        if (entity->id > 0 && entity->unk88 != 0) {
            entity->update(0xD, entity, 0, 0);
            entity->unk88 = 0;
            entity->unk90 = 0x7F00;
        }
    }
}
