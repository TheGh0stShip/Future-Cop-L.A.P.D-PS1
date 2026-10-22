#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
    u8 unk0C[0x0B];
    u8 group;
};

extern struct Entity *D_8009FA40;

extern u8 func_8004DA84(struct Entity *entity, s32 kind, s32 value, s32 retry);

void func_8004DD68(s32 group, s32 kind, s32 value)
{
    struct Entity *entity;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->group == group && entity->id != 0) {
            func_8004DA84(entity, kind, value, 1);
        }
    }
}
