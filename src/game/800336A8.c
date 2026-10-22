#include "../../include/game/types.h"

struct ReferencedObject {
    u8 pad[8];
    s32 value;
};

struct ObjectReference {
    u8 pad[0xC8];
    struct ReferencedObject *object;
    s32 value;
    u8 pad_D0[0x2A];
    u8 dirty;
};

void func_800336A8(struct ObjectReference *state,
                   struct ReferencedObject *object)
{
    if (object != 0 && state->object != object) {
        state->object = object;
        state->value = object->value;
        state->dirty = 0;
    }
}
