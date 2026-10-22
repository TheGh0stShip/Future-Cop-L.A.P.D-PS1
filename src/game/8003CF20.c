#include "game/types.h"

struct Unk3CF20 {
    struct Unk3CF20 *next;
    u8 unk04[0x30];
    void *buffer;
    u8 unk38;
    u8 flags;
};

extern void func_800181D4(void *block);

void func_8003CF20(struct Unk3CF20 *object)
{
    if (object->flags & 2) {
        func_800181D4(object->buffer);
    }
    func_800181D4(object);
}
