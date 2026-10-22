#include "game/types.h"

struct SoundVoice {
    s32 bank;
    s8 volume;
    s8 baseVolume;
    s8 pan;
    s8 extra;
    s32 priority;
    u8 unk0C[8];
    s32 *sample;
};

struct SoundBank {
    s32 samples[4];
    u8 count;
    u8 cursor;
};

extern void func_800477D8(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra);
extern u8 func_80024358(struct SoundBank *bank, struct SoundVoice *voice, s32 pan,
                        s32 volume, s32 extra);

u8 func_8004772C(struct SoundBank *bank, struct SoundVoice *voice, s32 pan, s32 volume,
                 s32 extra)
{
    u8 cursor;
    u8 result;

    if (bank->count != 0) {
        cursor = bank->cursor + 1;
        bank->cursor = cursor;
        if (cursor >= bank->count) {
            bank->cursor = 0;
        }
        voice->sample = &bank->samples[bank->cursor];
        func_800477D8(voice, pan, volume, extra);
        result = func_80024358(bank, voice, voice->pan, voice->volume, voice->extra);
    } else {
        result = 0;
    }
    return result;
}
