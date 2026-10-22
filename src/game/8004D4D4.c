#include "game/types.h"

struct EntityDef;

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
};

extern struct Entity *func_8004D444(struct EntityDef *def);

s32 func_8004D4D4(struct EntityDef *def)
{
    return func_8004D444(def)->id;
}
