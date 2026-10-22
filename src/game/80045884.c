#include "game/types.h"

struct Unk45884 {
    s32 position[3];
    s32 angle;
};

extern struct Unk45884 D_800A1920[];

void func_80045884(s32 *position, s32 arg1, s32 angle, s32 index)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        D_800A1920[index].position[i] = position[i];
    }
    angle = (angle + 0x1000) % 0x1000;
    D_800A1920[index].angle = angle;
}
