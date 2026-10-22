#include "../../include/game/types.h"

struct TaggedObject {
    u8 pad[0xD];
    s8 tag;
};

extern void func_800222CC(s32 tag, s32 *values);

void func_800243C4(struct TaggedObject *object, s32 third, s32 second)
{
    s32 values[3];

    values[0] = -1;
    values[1] = second;
    values[2] = third;
    func_800222CC(object->tag, values);
}
