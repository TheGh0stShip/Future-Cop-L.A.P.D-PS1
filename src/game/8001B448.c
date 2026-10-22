#include "game/types.h"

extern void *D_8009F5C8;
extern void *D_8009F5CC;
extern void *D_8009F5D0;
extern void *D_8009F5D4;
extern void *D_8009F5D8;
extern void *D_8009F5DC;

extern void func_800181D4(void *block);

void func_8001B448(void)
{
    func_800181D4(D_8009F5C8);
    func_800181D4(D_8009F5CC);
    func_800181D4(D_8009F5D0);
    func_800181D4(D_8009F5D4);
    func_800181D4(D_8009F5D8);
    func_800181D4(D_8009F5DC);
}
