#include "game/types.h"

struct SoundBank {
    u8 unk00[8];
    s32 handle;
};

struct SoundSystem {
    s32 bankCount;
    struct SoundBank **banks;
};

extern struct SoundSystem D_800A07A8;

extern void func_80098708(s32 handle);

void func_80021998(s32 index)
{
    struct SoundBank *bank;

    bank = D_800A07A8.banks[index];
    if (bank != 0) {
        func_80098708(bank->handle);
        bank->handle = -1;
    }
}
