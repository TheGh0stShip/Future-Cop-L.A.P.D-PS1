#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
    u8 unk0C[0x0D];
    u8 unk19;
    u8 unk1A[2];
    s16 unk1C;
    u8 unk1E[6];
    u32 flags;
};

struct EntityTable {
    s32 count;
    s32 unk04;
    struct Entity *items[1];
};

extern struct EntityTable D_800A8F90;

extern void func_8003BAE4(struct Entity *entity, s32 value);

void func_8004CE04(void)
{
    struct Entity *entity;
    s32 i;

    for (i = 0; i < D_800A8F90.count; i++) {
        entity = D_800A8F90.items[i];
        if (entity->id <= 0) {
            continue;
        }
        if (entity->unk1C < 0) {
            entity->update(4, entity, 0, 0);
        } else if (entity->flags & 0x4000) {
            entity->flags &= ~0x4000;
            func_8003BAE4(entity, (entity->unk19 >> 3) & 0x1E);
        }
    }
}
