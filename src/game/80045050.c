#include "game/types.h"

extern s32 func_800452C8(void);
extern void func_800452D4(void);

s8 func_80045050(void)
{
    s32 volume;

    volume = func_800452C8();
    if (volume != 0) {
        volume += volume >> 1;
        if (volume >= 0x80) {
            volume = 0x7F;
        }
        func_800452D4();
    }
    return volume;
}
