#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk56870 {
    u8 unk00[0x5C];
    struct Vec3s32 position;
};

extern u32 func_80019668(void);
extern void func_8005678C(struct Unk56870 *object, s32 index, struct Vec3s32 *out, s32 apply);
extern s32 func_80055EE8(struct Vec3s32 *from, struct Vec3s32 *to);

s32 func_80056870(struct Unk56870 *object)
{
    struct Vec3s32 out;
    struct Vec3s32 position;
    u32 slot;
    s32 tries;
    s32 blocked;

    slot = (func_80019668() & 7) + 9;
    position = object->position;
    for (tries = 8; tries > 0; tries--) {
        func_8005678C(object, slot, &out, 0);
        blocked = func_80055EE8(&position, &out);
        if (blocked == 0) {
            break;
        }
        slot++;
        if (slot >= 0x11) {
            slot = 9;
        }
    }
    if (blocked != 0) {
        return -1;
    }
    return slot;
}
