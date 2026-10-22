#include "game/types.h"

struct Unk5A654Sample {
    s16 id;
    u8 rate;
};

struct Unk5A6A4 {
    u8 unk00[0x15];
    u8 slot;
    u8 unk16[0x12];
    s16 scale;
};

extern struct Unk5A654Sample **D_8009F7D8;

extern s32 func_8005A654(s32 id);
extern s32 func_80049C38(struct Unk5A6A4 *object, s32 length);

s32 func_8005A6A4(s32 id, struct Unk5A6A4 *object, s32 *result)
{
    s32 index;
    s32 length;
    s32 value;

    index = func_8005A654(id);
    if (index < 0) {
        return 0;
    }
    object->slot = ~index;
    length = D_8009F7D8[index]->rate * object->scale;
    value = func_80049C38(object, length);
    if (result != 0) {
        *result = value;
    }
    return length;
}
