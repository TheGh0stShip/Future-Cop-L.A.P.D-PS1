#include "game/types.h"

struct Unk3C980Handler {
    void *unk00;
    void (*callback)(s32 message, struct Unk3C980Handler *handler, void *event,
                     s32 arg3);
};

struct Unk3C980Link {
    u8 unk00[4];
    struct Unk3C980Handler *handler;
    u8 unk08[0x30];
    u8 disabled;
};

struct Unk3C980Def {
    u8 unk00[0x2E];
    u16 eventId;
};

struct Unk3C980Object {
    u8 unk00[0x0F];
    u8 flags;
    u8 state;
    u8 unk11[0x0B];
    struct Unk3C980Link *link;
    struct Unk3C980Def *def;
};

struct Unk3C980Event {
    s32 unk00;
    u16 id;
    u8 enabled;
    u8 unk07;
    s32 unk08;
};

s32 func_8003C980(s32 message, struct Unk3C980Object *object)
{
    struct Unk3C980Handler *handler;
    struct Unk3C980Event event;
    u16 eventId;

    if (message == 7) {
        goto message7;
    }
    if ((u32)message < 8) {
        goto return0;
    }
    if (message == 8) {
        goto message8;
    }
    if (message == 9) {
        goto message9;
    }
    goto return0;

message8:
    object->flags |= 1;
    goto return0;

message7:
    if (object->state & 4) {
        object->flags |= 1;
    }
    return 1;

message9:
    handler = 0;
    if (object->link != 0 && object->link->disabled == 0) {
        handler = object->link->handler;
    }
    if (handler != 0) {
        eventId = object->def->eventId;
        if (eventId != 0) {
            event.id = eventId;
            event.unk00 = 0;
            event.unk08 = 0;
            event.enabled = 1;
            handler->callback(14, handler, &event, 0);
        }
    }

return0:
    return 0;
}
