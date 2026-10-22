#include "game/types.h"

struct SoundSystem {
    s32 bankCount;
    void **banks;
};

extern struct SoundSystem D_800A07A8;

extern void func_80021998(s32 index);

void func_80021DAC(s32 index)
{
    if (D_800A07A8.bankCount < index || index < 0 || D_800A07A8.banks == 0) {
        return;
    }
    func_80021998(index);
    D_800A07A8.banks[index] = 0;
}
