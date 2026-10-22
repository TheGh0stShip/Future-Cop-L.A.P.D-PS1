#include "game/types.h"

struct Matrix {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
};

extern void *D_8009FA1C;
extern void *D_8009F578;
extern void *D_8009F57C;
extern s16 D_8009F580;

extern void func_800959D0(s32 red, s32 green, s32 blue);
extern void func_800959A0(struct Matrix *matrix);
extern void *func_80017EBC(s32 size, s32 tag);

void func_80013A14(void)
{
    struct Matrix color;

    func_800959D0(0x40, 0x40, 0x40);
    color.m[0][0] = color.m[1][0] = color.m[2][0] = 0x1000;
    color.m[0][1] = color.m[0][2] = color.m[1][1] = color.m[1][2] = color.m[2][1] =
        color.m[2][2] = 0;
    func_800959A0(&color);
    D_8009FA1C = func_80017EBC(0x400, 2);
    D_8009F578 = func_80017EBC(0x800, 2);
    D_8009F57C = func_80017EBC(0x100, 2);
    D_8009F580 = 0;
}
