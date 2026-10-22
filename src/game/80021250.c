#include "game/types.h"

extern s32 D_8009F9AC;
extern s32 D_8009FA14;
extern s32 D_8009FA34;
extern s32 D_8009FAD0;
extern s32 D_8009F9C8;

extern void func_8002129C(void);
extern void func_80098F48(s32 value);
extern s32 func_80098FB8(s32 callback);

void func_80021250(void)
{
    if (D_8009F9C8 != 0) {
        func_8002129C();
        func_80098F48(D_8009FA14);
        func_80098FB8(D_8009F9AC);
    }
    D_8009FA34 = 0;
    D_8009FAD0 = 0;
    D_8009F9C8 = 0;
}
