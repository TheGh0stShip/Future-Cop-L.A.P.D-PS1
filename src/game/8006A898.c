#include "game/types.h"

struct Unk607B8 {
    u8 unk00[6];
    s16 unk06;
    s16 unk08;
    u8 unk0A[2];
    s16 unk0C;
};

struct Unk6A898 {
    u8 unk00[8];
    s16 unk08;
    s16 unk0A;
    struct Unk607B8 sub;
};

extern void func_800607B8(struct Unk607B8 *value);

void func_8006A898(struct Unk6A898 *object)
{
    func_800607B8(&object->sub);
    object->unk0A = (object->unk0A << 12) / 218;
    object->unk08 = (object->unk08 << 12) / 218;
}
