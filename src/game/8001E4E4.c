#include "game/types.h"

extern s32 D_8009FAD8;
extern u8 D_8009FA20;

extern void func_80091480(void);
extern void func_80095878(void);
extern void func_800200A8(void);
extern void func_8001E87C(void);
extern void func_8001E588(u32 port);

s32 func_8001E4E4(void)
{
    u32 port;

    func_80091480();
    func_80095878();
    func_800200A8();
    D_8009FAD8 = 0;
    func_8001E87C();
    D_8009FA20 = 0;
    for (port = 0; port < 2; port++) {
        func_8001E588(port);
    }
    return 0;
}
