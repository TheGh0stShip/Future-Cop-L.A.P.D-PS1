#include "game/types.h"

struct Unk3B3F8 {
    u8 unk00[0x10];
    u8 flags;
};

extern void *D_800A7040[][0x100];
extern u8 D_800A6578[][0x100][4];

void func_8003B3F8(struct Unk3B3F8 *object, u32 column, s32 row)
{
    column &= 0xFF;
    if (D_800A7040[row][column] != 0) {
        object->flags |= D_800A6578[row][column][0];
    }
}
