#include "game/types.h"

extern s32 D_8009FA00[];
extern s32 D_8009FAB8;
extern s32 D_8009F760;
extern s32 D_8009FAC8;

extern u8 func_80048790(void);

void func_800487CC(s32 input)
{
    register s32 cycle __asm__("$16");
    register s32 force __asm__("$17");
    s32 current;

    force = input;
    if ((u8)force && D_8009FA00[0] == 5) {
        force = 0;
    }
    current = D_8009FAC8;
    if (current != 0 && D_8009F760 != current) {
        D_8009FAB8 = 2;
        force = 0;
    }
    D_8009F760 = current;
    cycle = 0;
    if (func_80048790() || (u8)force) {
        cycle = 1;
    }
    if ((u8)cycle) {
        do {
            D_8009FAB8++;
            if (D_8009FAB8 >= 3) {
                D_8009FAB8 = 0;
            }
        } while (func_80048790());
    }
}
