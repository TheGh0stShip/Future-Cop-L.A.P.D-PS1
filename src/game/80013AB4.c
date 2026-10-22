#include "game/types.h"

extern void *D_8009FA1C;
extern void *D_8009F578;
extern void *D_8009F57C;

extern void func_800181D4(void *block);

void func_80013AB4(void)
{
    func_800181D4(D_8009FA1C);
    func_800181D4(D_8009F578);
    func_800181D4(D_8009F57C);
}
