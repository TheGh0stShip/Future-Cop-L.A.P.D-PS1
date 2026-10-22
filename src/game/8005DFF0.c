#include "game/types.h"

struct Unk5DFF0 {
    s32 model;
    u8 unk04[0x14];
    u8 coord[0x30];
    s16 size;
    u8 unk4A[8];
    u16 flags;
    u8 unk54[0x154];
    u8 baseRed;
    u8 baseGreen;
    u8 baseBlue;
    u8 flashRed;
    u8 flashGreen;
    u8 flashBlue;
    s8 flash;
};

extern void func_8006CB58(s32 model, u8 *coord, s32 size, s32 arg3, s32 arg4, s32 arg5,
                          s32 red, s32 green, s32 blue);

void func_8005DFF0(struct Unk5DFF0 *effect)
{
    u8 red;
    u8 green;
    u8 blue;

    if (!(effect->flags & 1)) {
        return;
    }
    if (effect->flash > 0) {
        red = effect->flashRed;
        green = effect->flashGreen;
        blue = effect->flashBlue;
        effect->flash--;
    } else {
        red = effect->baseRed;
        green = effect->baseGreen;
        blue = effect->baseBlue;
    }
    func_8006CB58(effect->model, effect->coord, effect->size, 0, 0, 0, red, green, blue);
}
