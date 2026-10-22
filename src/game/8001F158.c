#include "game/types.h"

extern s32 D_8009F99C;
extern u8 D_8009FA20;

extern s32 func_80091510(void (*callback)(void));

void func_8001F158(void)
{
    func_80091510(0);
    D_8009F99C = 0;
    D_8009FA20 = 0;
}
