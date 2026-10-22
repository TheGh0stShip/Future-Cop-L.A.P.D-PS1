#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
    u8 unk0C[8];
    u16 tag;
};

extern struct Entity *D_8009FA40;

s32 func_8004DE80(s32 tag, s32 arg)
{
    struct Entity *entity;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->tag == tag) {
            return entity->update(0xC, entity, arg, 0);
        }
    }
    return 0;
}
