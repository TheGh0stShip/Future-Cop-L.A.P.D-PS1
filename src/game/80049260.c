#include "game/types.h"

extern u32 D_8009FACC;

extern s32 func_80046C84(s32 id, s32 arg1);

void func_80049260(void)
{
    if (D_8009FACC != 0) {
        if (D_8009FACC < 0x82) {
            func_80046C84(D_8009FACC, 0);
        }
        D_8009FACC = 0;
    }
}
