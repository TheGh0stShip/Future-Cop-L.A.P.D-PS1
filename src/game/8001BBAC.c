#include "game/types.h"

extern s32 D_8009F5C0;

extern void func_8001B4A8(s32 arg0);

void func_8001BBAC(void)
{
    func_8001B4A8(0);
    if (++D_8009F5C0 >= 2) {
        D_8009F5C0 = 0;
    }
}
