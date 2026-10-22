#include "game/types.h"

struct Entity {
    struct Entity *next;
    s32 (*update)(s32 message, struct Entity *entity, s32 arg2, s32 arg3);
    s32 id;
    u8 unk0C[0x18];
    u32 flags;
};

extern struct Entity *D_8009FA40;
extern s32 D_8009F7A0;
extern void *D_8009FA58[];

extern void func_8004D958(struct Entity *entity);
extern void func_8004D598(void);

void func_8004CCDC(void)
{
    struct Entity *entity;
    s32 i;

    for (entity = D_8009FA40; entity != 0; entity = entity->next) {
        if (entity->id != 0) {
            entity->flags |= 0x4000000;
            func_8004D958(entity);
        }
    }
    func_8004D598();
    D_8009F7A0 = 0;
    for (i = 1; i >= 0; i--) {
        D_8009FA58[i] = 0;
    }
}
