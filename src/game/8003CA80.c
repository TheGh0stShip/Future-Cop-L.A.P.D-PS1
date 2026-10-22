#include "game/types.h"

struct Unk3CA80 {
    u8 unk00[4];
    s16 unk04;
    u8 unk06;
    s8 unk07;
    u8 unk08[7];
    u8 flags;
    u8 unk10[4];
    s16 unk14;
    u8 unk16[0x0E];
    void (*callback)(s32 message, struct Unk3CA80 *object, s32 arg2, s32 arg3);
};

void func_8003CA80(struct Unk3CA80 *object)
{
    object->callback(7, object, 0, 0);
    object->unk07 = -1;
    object->unk14 = 0;
    object->unk06 = 0;
    object->unk04 = -1;
    object->flags |= 2;
}
