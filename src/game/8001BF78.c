#include "game/types.h"

extern u8 D_8009F225;

extern void func_80091F60(s32 arg0);
extern void func_8001BE68(void);

void func_8001BF78(void)
{
    func_80091F60(0);
    D_8009F225 = 2;
    func_8001BE68();
}
