#include "game/types.h"

struct Unk607B8 {
    u8 unk00[6];
    s16 unk06;
    s16 unk08;
    u8 unk0A[2];
    s16 unk0C;
};

void func_800607B8(struct Unk607B8 *value)
{
    value->unk06 = value->unk06 / 300;
    if (value->unk08 != 0) {
        value->unk08 = (value->unk08 << 8) / 300;
    }
    value->unk0C = (value->unk0C << 12) / 218;
}
