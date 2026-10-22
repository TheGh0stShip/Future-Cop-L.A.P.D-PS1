#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
    u8 unk0C[8];
    u16 tag;
};

extern struct Entity *D_8009FA40;

struct Entity *func_8004D69C(s32 tag)
{
    struct Entity *entity;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->tag == tag) {
            break;
        }
    }
    return entity;
}
