#include "game/types.h"

extern s32 D_8009F9AC;
extern s32 D_8009FA14;
extern s32 D_8009FA80;
extern s32 D_8009F998;
extern s32 D_8009FA34;
extern s32 D_8009FAD0;
extern s32 D_8009F9C8;

extern void D_8002139C(void);
extern s32 func_80098FB8(void (*callback)(void));
extern s32 func_80098F78(void);
extern void func_80098F48(s32 value);

void func_800211FC(void)
{
    D_8009F9AC = func_80098FB8(D_8002139C);
    D_8009FA14 = func_80098F78();
    func_80098F48(0);
    D_8009FA80 = 0;
    D_8009F998 = 0;
    D_8009FA34 = 0;
    D_8009FAD0 = 0;
    D_8009F9C8 = 1;
}
