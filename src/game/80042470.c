#include "game/types.h"

struct Unk423F0Info {
    u8 unk00[0x300];
    void *target;
};

struct Unk423F0Player {
    u8 unk00[0xAC];
    struct Unk423F0Info *info;
};

struct Unk423F0Object {
    u8 unk00[0xA0];
    s8 side;
};

extern struct Unk423F0Player *D_8009FA58[];

s32 func_80042470(s32 message, struct Unk423F0Object *object)
{
    s32 first;
    s32 last;
    s32 i;

    if (object->side == 0) {
        first = 0;
        last = 0;
    } else if (object->side == 1) {
        first = 1;
        last = 1;
    } else {
        first = 0;
        last = 1;
    }
    for (i = first; i <= last; i++) {
        if (D_8009FA58[i] != 0 && D_8009FA58[i]->info != 0 &&
            D_8009FA58[i]->info->target == object) {
            D_8009FA58[i]->info->target = 0;
        }
    }
    return 0;
}
