#include "game/types.h"

extern s32 D_8009F730;
extern s32 D_8009F334;

extern void func_80024730(s32 volume);

void func_800451B4(s32 enable)
{
    D_8009F730 = enable != 0;
    func_80024730(D_8009F334);
}
