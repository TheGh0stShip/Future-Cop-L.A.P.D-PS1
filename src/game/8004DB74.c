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
    s32 i;
    u32 pending;
    struct EntityMessage *message;
    u32 bit;

    if (D_8009F7A8 == 0) {
        return;
    }
    pending = D_8009F7A8;
    message = D_800A2090;
    i = 0;
    bit = 1;
    do {
        if (!(pending & 1)) {
            goto next;
        }
        message->timer -= elapsed;
        if (message->timer > 0) {
            goto next;
        }
        message->timer += 600;
        if (message->target->id == message->id &&
            func_8004DA84(message->target, message->kind, message->value, 0) == 0) {
            goto next;
        }
        D_8009F7A8 ^= bit << i;
next:
        i++;
        message++;
        pending >>= 1;
    } while (pending != 0);
}
