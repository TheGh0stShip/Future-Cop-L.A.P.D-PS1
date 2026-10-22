#include "game/types.h"

struct Entity;

struct EntityDef {
    s32 (*handler)(s32 message, struct Entity *entity, struct EntityDef *def, s32 arg3);
};

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, struct EntityDef *def, s32 arg3);
    s32 id;
};

extern s32 D_8009F7A0;

extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8004D3D4(struct Entity *entity);

struct Entity *func_8004D444(struct EntityDef *def)
{
    struct Entity *entity;
    s32 (*handler)(s32, struct Entity *, struct EntityDef *, s32);

    entity = func_80017EBC(def->handler(1, 0, 0, 0), 3);
    handler = def->handler;
    entity->update = handler;
    entity->id = ++D_8009F7A0;
    handler(2, entity, def, 0);
    func_8004D3D4(entity);
    return entity;
}
