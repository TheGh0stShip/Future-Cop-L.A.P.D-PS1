#include "game/types.h"

struct SoundVoice {
    s32 bank;
    s8 volume;
    s8 baseVolume;
    s8 pan;
    s8 extra;
    u8 unk08[5];
    s8 voice;
};

struct StreamChannel {
    s32 id;
    struct SoundVoice *voice;
    s32 left;
    s32 right;
    u8 unk10[0x14];
};

struct StreamState {
    u8 unk00[4];
    s32 active;
    u8 unk08[0x1C];
    struct StreamChannel channels[3];
};

struct StreamMix {
    s32 unk00;
    s32 volume;
    s32 pan;
};

extern struct StreamState D_800A0968;

extern void func_800227BC(s32 channel, s32 left, s32 right);
extern void func_800477D8(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra);
extern void func_800222CC(s32 voice, struct StreamMix *mix);

void func_8002283C(s32 channel, s32 left, s32 right)
{
    struct SoundVoice *voice;
    struct StreamMix mix;
    s32 clamped;

    if (channel >= 0) {
        if (channel < 3) {
            clamped = channel;
        } else {
            clamped = 2;
        }
    } else {
        clamped = 0;
    }
    channel = clamped;
    func_800227BC(channel, left, right);
    if (D_800A0968.active == 0) {
        return;
    }
    voice = D_800A0968.channels[channel].voice;
    if (voice == 0) {
        return;
    }
    func_800477D8(voice, D_800A0968.channels[channel].right, D_800A0968.channels[channel].left, -1);
    mix.volume = voice->volume;
    mix.pan = voice->pan;
    mix.unk00 = -1;
    func_800222CC(voice->voice, &mix);
}
