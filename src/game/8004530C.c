#include "game/types.h"

struct SoundVoice {
    u8 unk00[5];
    s8 note;
    s8 program;
    s8 velocity;
    u8 unk08[0x14];
    struct SoundVoice *next;
    u8 unk20[0x0C];
};

extern void func_80047800(struct SoundVoice *voice, s32 program, s32 note, s32 velocity);

void func_8004530C(struct SoundVoice *voice)
{
    while (voice != 0) {
        if (voice->note != -1) {
            func_80047800(voice, voice->program, voice->note, voice->velocity);
        }
        voice = voice->next;
    }
}
