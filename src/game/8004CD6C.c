#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
};

struct EntityTable {
    s32 count;
    s32 unk04;
    struct Entity *items[1];
};

extern struct EntityTable D_800A8F90;

void func_8004CD6C(s32 message, s32 arg2, s32 arg3)
{
    struct Entity **cursor;
    struct Entity *entity;
    s32 remaining;

    cursor = D_800A8F90.items;
    for (remaining = D_800A8F90.count; remaining > 0; remaining--) {
        entity = *cursor++;
        if (entity->id > 0) {
            entity->update(message, entity, arg2, arg3);
        }
    }
}
