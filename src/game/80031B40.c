#include "game/types.h"

struct Unk31Node {
    s16 x;
    s16 unk02;
    s16 z;
    s16 unk06;
    s32 id;
    s16 extentX;
    s16 unk0E;
    s16 extentZ;
    s16 unk12;
    u8 flags;
    u8 unk15;
    s16 unk16;
    u8 unk18[0x0C];
    struct Unk31Node *next;
};

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

extern struct Unk31Node *D_8009F6F8;

s32 func_80031B40(struct Vec3s32 *position, s32 radius)
{
    struct Unk31Node *node;
    s32 found;
    s32 delta;
    s32 sign;

    found = 0;
    for (node = D_8009F6F8; node != 0; node = node->next) {
        if (node->flags & 0x40) {
            delta = position->x - (node->x << 8);
            sign = delta >> 31;
            if (((sign + delta) ^ sign) - radius <= node->extentX << 4) {
                delta = position->z - (node->z << 8);
                sign = delta >> 31;
                if (((sign + delta) ^ sign) - radius <= node->extentZ << 4) {
                    found = 1;
                    break;
                }
            }
        }
    }
    return found;
}
