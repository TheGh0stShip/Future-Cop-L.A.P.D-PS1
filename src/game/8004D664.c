#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
};

extern struct Entity *D_8009FA40;

struct Entity *func_8004D664(s32 id)
{
    struct Entity *entity;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->id == id) {
            break;
        }
    }
    return entity;
}
