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
extern s32 D_8009F7A8;

s32 func_8004D9EC(struct Entity *target, u8 kind, u8 value)
{
    struct EntityMessage *message;
    s32 bit;

    if (D_8009F7A8 == 0xFFFF) {
        return 0;
    }
    message = D_800A2090;
    bit = 1;
    while (D_8009F7A8 & bit) {
        bit <<= 1;
        message++;
    }
    message->target = target;
    D_8009F7A8 |= bit;
    message->kind = kind;
    message->value = value;
    message->timer = 600;
    message->id = target->id;
    return 1;
}
