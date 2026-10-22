#include "game/types.h"

struct Entity {
    struct Entity *next;
    void *update;
    s32 id;
};

struct EntityMessage {
    u8 kind;
    u8 value;
    s16 timer;
    struct Entity *target;
    s32 id;
};

extern struct EntityMessage D_800A2090[];
extern u32 D_8009F7A8;

extern u8 func_8004DA84(struct Entity *entity, u8 kind, u8 value, s32 retry);

void func_8004DB74(s32 elapsed)
{
    struct EntityMessage *message;
    u32 pending;
    s32 i;

    pending = D_8009F7A8;
    message = D_800A2090;
    for (i = 0; pending != 0; i++, message++, pending >>= 1) {
        if (!(pending & 1)) {
            continue;
        }
        message->timer -= elapsed;
        if (message->timer > 0) {
            continue;
        }
        message->timer += 600;
        if (message->target->id == message->id &&
            func_8004DA84(message->target, message->kind, message->value, 0) == 0) {
            continue;
        }
        D_8009F7A8 ^= 1 << i;
    }
}
