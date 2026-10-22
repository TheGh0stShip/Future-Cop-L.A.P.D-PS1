#include "game/types.h"

struct SoundVoiceSlot {
    s32 active;
    u8 unk04[0x14];
    s32 owner;
};

struct SoundSystem {
    s32 bankCount;
    void **banks;
    s32 channelCount;
    void *channels;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    struct SoundVoiceSlot voices[3];
    s32 volume;
};

extern struct SoundSystem D_800A07A8;

void func_80021508(void)
{
    s32 i;

    D_800A07A8.bankCount = 0;
    D_800A07A8.banks = 0;
    D_800A07A8.channelCount = 0;
    D_800A07A8.channels = 0;
    D_800A07A8.unk10 = 0;
    D_800A07A8.unk14 = 0;
    D_800A07A8.unk18 = 0;
    for (i = 2; i >= 0; i--) {
        D_800A07A8.voices[i].active = 0;
    }
    D_800A07A8.volume = 0x7F;
}
