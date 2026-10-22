#include "game/types.h"

extern void *D_8009FA4C;
extern void *D_8009FAAC;
extern void *D_8009F9D0;

extern void func_800181D4(void *block);

void func_80017D20(void)
{
    func_800181D4(D_8009FA4C);
    func_800181D4(D_8009FAAC);
    func_800181D4(D_8009F9D0);
}
