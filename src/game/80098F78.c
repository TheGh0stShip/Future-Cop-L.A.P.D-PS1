#include "game/types.h"

extern s32 D_8009EA34;
extern s32 D_8009EEC0;

s32 func_80098F78(void)
{
    s32 mode;

    mode = D_8009EEC0;
    switch (mode) {
    case 1:
        D_8009EA34 = mode;
        break;
    case 0:
    default:
        D_8009EA34 = 0;
        break;
    }

    return D_8009EA34;
}
