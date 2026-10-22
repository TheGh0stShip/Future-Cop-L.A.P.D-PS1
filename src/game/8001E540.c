#include "game/types.h"

extern u8 D_8009FA20;

extern void func_800200F0(void);
extern void func_8001E910(void);
extern void func_8001F158(void);
extern void func_80091574(void);

s32 func_8001E540(void)
{
    func_800200F0();
    func_8001E910();
    if (D_8009FA20 != 0) {
        func_8001F158();
    }
    func_80091574();
    return 0;
}
