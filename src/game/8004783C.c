#include "game/types.h"

extern s32 D_8009FAB8;
extern s32 D_8009F760;
extern s32 D_8009FAC8;

extern void func_80048EC4(void);

void func_8004783C(void)
{
    D_8009FAB8 = 0;
    D_8009F760 = 0;
    D_8009FAC8 = 0;
    func_80048EC4();
}
