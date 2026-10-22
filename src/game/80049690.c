#include "game/types.h"

extern s32 D_8009F794;
extern s32 D_8009F788;
extern void *D_8009F778;
extern void *D_8009F780;
extern s32 D_8009F784;
extern void *D_8009F77C;

extern s32 func_800194DC(s32 count, s32 size, s32 tag);

void func_80049690(void)
{
    D_8009F794 = 0;
    D_8009F788 = 0;
    D_8009F778 = 0;
    D_8009F780 = 0;
    D_8009F784 = func_800194DC(0x180, 0x44, 2);
    D_8009F77C = 0;
}
