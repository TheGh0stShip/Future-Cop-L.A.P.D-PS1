#include "game/types.h"

extern u8 D_800A085C[];

extern void func_80097A28(void);
extern void func_80096E40(void);
extern void func_80097AC8(s32 count, u8 *table);
extern void func_80021824(void);

void func_80021B50(void)
{
    func_80097A28();
    func_80096E40();
    func_80097AC8(0x20, D_800A085C);
    func_80021824();
}
