#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
    u8 unk0C[0x16];
    s8 sound;
    u8 unk23;
    u32 flags;
    u8 unk28[0x70];
    s32 script;
};

extern s32 func_80069D68(s32 script, s32 arg1, s32 arg2, struct Entity *entity);
extern void func_800457B8(s32 arg0, s32 index);

void func_8004D958(struct Entity *entity)
{
    if (entity->id == 0) {
        return;
    }
    if (!(entity->flags & 0x8000000) && entity->script != 0) {
        func_80069D68(entity->script, ((entity->flags >> 26) ^ 1) & 1, -1, entity);
    }
    if (entity->flags & 0x400000) {
        func_800457B8(entity->id, entity->sound);
    }
    entity->id = 0;
}
