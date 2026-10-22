#include "game/types.h"

extern void *D_8009F808;
extern void *D_8009F810;
extern void *D_8009FA08;
extern void *D_800A294C[];
extern void func_800181D4(void *block);

void func_800634F4(void)
{
    if (D_8009F808 != 0) {
        func_800181D4(D_8009F808);
    }
    if (D_8009F810 != 0) {
        func_800181D4(D_8009F810);
    }
    if (D_8009FA08 != 0) {
        func_800181D4(D_8009FA08);
    }
    if (D_800A294C[0] != 0) {
        func_800181D4(D_800A294C[0]);
    }
}
