#include "game/types.h"

struct FontEntry;

extern u16 D_800A1940[];
extern u16 D_800A1970[];
extern u32 D_8009FACC[];
extern u16 D_8009F76A;
extern u16 D_8009F768;
extern struct FontEntry *D_8009F35C;
extern s32 D_8009F360;
extern s32 D_8009F364;

extern void func_80048978(s32 blue, s32 green, s32 red);

void func_80048EC4(void)
{
    s32 i;

    D_8009F76A = 1;
    D_8009F768 = 0;
    for (i = 0; i < 0x14; i++) {
        D_800A1940[i] = 0;
        D_800A1970[i] = 0;
    }
    D_8009F35C = 0;
    D_8009F360 = 0;
    D_8009F364 = 0;
    D_8009FACC[0] = 0;
    func_80048978(0x1F, 0x1F, 0x1F);
}
