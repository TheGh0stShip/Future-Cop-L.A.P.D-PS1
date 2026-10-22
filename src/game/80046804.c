#include "game/types.h"

struct SoundVoice {
    s32 bank;
    s8 volume;
    s8 baseVolume;
    s8 pan;
    s8 extra;
    s32 priority;
    u8 unk0C[0x18];
    struct SoundVoice *bankNext;
    struct SoundVoice *bankPrev;
};

struct SoundBank {
    s32 samples[4];
    u8 count;
    u8 cursor;
    u8 voices;
    u8 unk13;
    u8 unk14;
    u8 unk15;
    u8 key;
    u8 unk17;
    struct SoundVoice *head;
};

extern struct SoundBank D_800A7938[];

void func_80046804(struct SoundVoice *voice)
{
    struct SoundBank *bank;

    bank = &D_800A7938[voice->bank];
    if (voice->bankPrev != 0) {
        voice->bankPrev->bankNext = voice->bankNext;
    } else {
        bank->head = voice->bankNext;
    }
    if (voice->bankNext != 0) {
        voice->bankNext->bankPrev = voice->bankPrev;
    }
    bank->voices--;
}
