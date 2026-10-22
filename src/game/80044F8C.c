#include "game/types.h"

extern s32 D_800A1800[];
extern u8 D_8009F739;
extern u8 D_8009F340;

extern void func_800451B4(s32 value);
extern void func_800451EC(s32 value);
extern void func_800451F8(s32 value);
extern void func_80045238(s32 value);
extern void func_80045294(s32 value);
extern void func_800453B8(void);

void func_80044F8C(void)
{
    func_800451B4(1);
    func_800451EC(0);
    func_800451F8(0x40);
    func_80045238(0x40);
    func_80045294(0x40);
    func_800453B8();
    D_800A1800[0] = -1;
    D_8009F739 = 0;
    D_8009F340 = 1;
}
