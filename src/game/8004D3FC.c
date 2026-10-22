#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
};

extern s32 D_8009F7A0;

extern void *func_80017EBC(s32 size, s32 tag);
extern void func_8004D3D4(struct Entity *entity);

struct Entity *func_8004D3FC(s32 size)
{
    struct Entity *entity;

    entity = func_80017EBC(size, 3);
    entity->id = ++D_8009F7A0;
    func_8004D3D4(entity);
    return entity;
}
