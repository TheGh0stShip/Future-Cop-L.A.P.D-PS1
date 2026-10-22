#include "game/types.h"

extern u8 func_80024170(void);
extern u8 func_80029BC0(void);
extern s32 func_80024188(void);
extern void func_80047554(s32 value);

void func_800476D0(void)
{
    s32 value;

    value = 0;
    if (func_80024170() != 0 && func_80029BC0() == 0) {
        value = func_80024188();
    }
    func_80047554(value);
}
