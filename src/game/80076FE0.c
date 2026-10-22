#include "game/types.h"

extern s32 D_8009FA94;
extern s32 D_8009FAD4;
extern s32 D_8009F9A4;
extern s32 D_8009F9F8;

extern void func_80023478(void);
extern void func_8001E2C0(s32 block);
extern void func_8001E540(void);
extern void func_80017BA0(void);

void func_80076FE0(void)
{
    func_80023478();
    func_8001E2C0(D_8009FA94);
    func_8001E2C0(D_8009FAD4);
    func_8001E2C0(D_8009F9A4);
    func_8001E2C0(D_8009F9F8);
    func_8001E540();
    func_80017BA0();
}
