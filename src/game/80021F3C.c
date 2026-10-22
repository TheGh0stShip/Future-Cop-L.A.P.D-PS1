#include "game/types.h"

struct SoundChannelSlot {
    u8 unk00[0x0C];
    s32 channel;
    u8 unk10[0x0C];
};

struct SoundSystem {
    s32 bankCount;
    void **banks;
    s32 channelCount;
    struct SoundChannelSlot *channels;
};

extern struct SoundSystem D_800A07A8;

extern void func_80021AB4(u32 mask);
extern void func_80021E1C(void);

void func_80021F3C(void)
{
    struct SoundChannelSlot *slot;
    s32 remaining;
    u32 mask;

    remaining = D_800A07A8.channelCount;
    slot = D_800A07A8.channels;
    mask = 0;
    for (; remaining != 0; remaining--) {
        mask |= 1 << slot->channel;
        slot++;
    }
    func_80021AB4(mask);
    func_80021E1C();
}
