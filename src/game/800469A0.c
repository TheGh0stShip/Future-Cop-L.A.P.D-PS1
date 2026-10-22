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

void func_800469A0(struct SoundVoice *voice)
{
    struct SoundBank *bank;
    struct SoundVoice *prev;
    struct SoundVoice *next;

    bank = &D_800A7938[voice->bank];
    prev = 0;
    for (next = bank->head; next != 0; next = next->bankNext) {
        if (voice->priority < next->priority) {
            break;
        }
        prev = next;
    }
    voice->bankPrev = prev;
    voice->bankNext = next;
    if (prev != 0) {
        prev->bankNext = voice;
    } else {
        bank->head = voice;
    }
    if (next != 0) {
        next->bankPrev = voice;
    }
    bank->voices++;
}
