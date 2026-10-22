#include "game/types.h"

struct SoundVoice {
    s32 bank;
    s8 volume;
    s8 baseVolume;
    s8 pan;
    s8 extra;
};

extern void func_800477D8(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra);
extern void func_800243C4(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra);

void func_80047800(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra)
{
    func_800477D8(voice, pan, volume, extra);
    func_800243C4(voice, voice->pan, voice->volume, voice->extra);
}
