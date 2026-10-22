#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
    u8 unk0C[0x17];
    u8 unk23;
    u32 flags;
    s32 unk28;
};

extern struct Entity *D_8009FA40;

extern void func_80065834(struct Entity *entity, s32 arg1, s32 arg2, s32 arg3);

void func_8004ED18(s32 arg1, s32 arg2, s32 arg3)
{
    struct Entity *entity;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->id > 0 && entity->unk23 != 0 && !(entity->flags & 0x30) &&
            entity->unk28 != 0) {
            func_80065834(entity, arg1, arg2, arg3);
        }
    }
}
