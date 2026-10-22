#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
    u8 unk0C[0x10];
    s16 unk1C;
    u8 unk1E[0x7E];
    u8 *script;
};

extern s32 func_80069D68(u8 *script, s32 arg1, s32 arg2, struct Entity *entity);

void func_8004DEE0(struct Entity *entity)
{
    if (entity->script != 0 && entity->script[0] != 0) {
        func_80069D68(entity->script, entity->unk1C < 0, -1, entity);
    }
}
