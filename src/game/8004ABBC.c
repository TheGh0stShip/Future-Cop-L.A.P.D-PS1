#include "game/types.h"

extern void *D_8009F79C;

extern void func_800181D4(void *block);

void func_8004ABBC(void)
{
    func_800181D4(D_8009F79C);
    D_8009F79C = 0;
}
