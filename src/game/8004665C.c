#include "game/types.h"

struct SoundVoice {
    u8 unk00[0x1C];
    struct SoundVoice *next;
};

extern struct SoundVoice *D_8009F73C;
extern s32 D_8009F740;

struct SoundVoice *func_8004665C(void)
{
    struct SoundVoice *voice;

    if (D_8009F740 != 0) {
        D_8009F740--;
        voice = D_8009F73C;
        D_8009F73C = voice->next;
    } else {
        voice = 0;
    }
    return voice;
}
