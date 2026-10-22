#include "game/types.h"

extern s32 D_8009FA00[];
extern s32 D_8009FAB8;
extern s32 D_8009F760;
extern s32 D_8009FAC8;

extern u8 func_80048790(void);

void func_800487CC(u8 force)
{
    u8 cycle;

    if (force && D_8009FA00[0] == 5) {
        force = 0;
    }
    if (D_8009FAC8 != 0 && D_8009F760 != D_8009FAC8) {
        D_8009FAB8 = 2;
        force = 0;
    }
    D_8009F760 = D_8009FAC8;
    cycle = 0;
    if (func_80048790() || force) {
        cycle = 1;
    }
    if (cycle) {
        do {
            D_8009FAB8++;
            if (D_8009FAB8 >= 3) {
                D_8009FAB8 = 0;
            }
        } while (func_80048790());
    }
}
