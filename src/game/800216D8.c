#include "game/types.h"

struct SoundSystem {
    s32 bankCount;
    void **banks;
    s32 channelCount;
    void *channels;
};

extern struct SoundSystem D_800A07A8;

extern void func_800217A8(void);

void func_800216D8(void)
{
    if (D_800A07A8.banks != 0) {
        func_800217A8();
        D_800A07A8.banks = 0;
        D_800A07A8.bankCount = 0;
    }
    if (D_800A07A8.channels != 0) {
        D_800A07A8.channels = 0;
        D_800A07A8.channelCount = 0;
    }
}
