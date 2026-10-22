#include "../../include/game/types.h"

extern s32 D_8009EA34;
extern s32 D_8009EEC0;

void func_80098F48(s32 mode)
{
    s32 enabled;

    switch (mode) {
    case 0:
        enabled = 0;
        break;
    case 1:
        enabled = 1;
        break;
    default:
        enabled = 0;
        break;
    }
    D_8009EA34 = mode;
    D_8009EEC0 = enabled;
}
