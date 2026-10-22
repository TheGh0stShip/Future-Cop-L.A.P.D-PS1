#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
    u8 unk0C[0x18];
    u32 flags;
};

extern struct Entity *D_8009FA40;
extern struct Entity *D_8009FA88;

extern void func_80061020(struct Entity *entity);
extern void func_800181D4(void *block);

void func_8004D598(void)
{
    struct Entity *entity;
    struct Entity *next;
    struct Entity *previous;

    previous = 0;
    next = D_8009FA40;
    while (next != 0) {
        entity = next;
        next = next->next;
        if (entity->id == 0) {
            if (entity->flags & 0x10000000) {
                func_80061020(entity);
            }
            entity->update(5, entity, 0, 0);
            if (previous != 0) {
                if ((previous->next = entity->next) != 0) {
                    goto release;
                }
            } else {
                if ((D_8009FA40 = entity->next) != 0) {
                    goto release;
                }
            }
            D_8009FA88 = previous;
release:
            func_800181D4(entity);
        } else {
            previous = entity;
        }
    }
}
