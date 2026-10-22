#include "game/types.h"

struct Unk4B244Source {
    u8 unk00[0x1A];
    u16 flags;
};

struct Unk4B244Force {
    s32 unk00;
    s32 x;
    s32 y;
    s32 z;
    u8 unk10[0x30];
    u16 strength;
    u8 unk42[7];
    u8 flags;
};

s32 func_8004B244(struct Unk4B244Source *source, struct Unk4B244Force *force, s32 *out)
{
    s16 scale;
    u16 flags;

    if (force == 0) {
        return 0;
    }
    if (!(force->flags & 2)) {
        return 0;
    }
    flags = source->flags;
    scale = 0;
    if (!(flags & 0x200)) {
        if (flags & 0x400) {
            scale = -(force->strength << 3);
        } else {
            scale = force->strength << 3;
        }
    }
    if (scale == 0) {
        return 0;
    }
    out[0] = ((force->x >> 4) * scale) >> 8;
    out[1] = ((force->y >> 4) * scale) >> 8;
    out[2] = ((force->z >> 4) * scale) >> 8;
    return 1;
}
