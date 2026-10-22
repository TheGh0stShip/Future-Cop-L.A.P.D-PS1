#include "game/types.h"

struct SoundSystem {
    s32 bankCount;
};

extern struct SoundSystem D_800A07A8;

extern void func_80021DAC(s32 index);

void func_800217A8(void)
{
    s32 i;

    for (i = 0; i < D_800A07A8.bankCount; i++) {
        func_80021DAC(i);
    }
}
