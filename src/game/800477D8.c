#include "game/types.h"

struct SoundVoice {
    s32 bank;
    s8 volume;
    s8 baseVolume;
    s8 pan;
    s8 extra;
};

extern u8 D_8009F738;

void func_800477D8(struct SoundVoice *voice, s32 pan, s32 volume, s32 extra)
{
    voice->baseVolume = volume;
    if (D_8009F738 != 0) {
        volume >>= 1;
    }
    voice->volume = volume;
    voice->pan = pan;
    voice->extra = extra;
}
