#include "game/types.h"

struct Entity {
    struct Entity *next;
};

extern struct Entity *D_8009FA40;
extern struct Entity *D_8009FA88;

void func_8004D3D4(struct Entity *entity)
{
    if (D_8009FA88 != 0) {
        D_8009FA88->next = entity;
    } else {
        D_8009FA40 = entity;
    }
    D_8009FA88 = entity;
    entity->next = 0;
}
