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
};

extern struct SoundSystem D_800A07A8;

extern void func_8002238C(s32 channel);

void func_80022240(s32 owner)
{
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_800A07A8.voices[i].active != 0 && D_800A07A8.voices[i].owner == owner) {
            func_8002238C(owner);
            D_800A07A8.voices[i].active = 0;
            D_800A07A8.voices[i].owner = -1;
        }
    }
}
