#include "game/types.h"

struct SoundChannel {
    s32 id;
    s32 priority;
    s16 slot;
    s8 index;
    u8 unk0B;
    s32 unk0C;
    s32 unk10;
    u8 unk14[0x14];
    s32 unk28;
    struct SoundChannel *next;
    struct SoundChannel *prev;
};

extern struct SoundChannel D_800A1160[];

s32 func_80045768(s32 arg0, s32 index)
{
    register struct SoundChannel *base __asm__("$3");
    struct SoundChannel *channel;
    register s32 active __asm__("$2");
    register s32 result __asm__("$6");

    result = 0;
    if (index >= 0) {
        base = D_800A1160;
        channel = &base[index];
        if (channel->id != -1) {
            active = channel->priority != 0;
            __asm__ volatile ("" : "+r" (active));
            result = active;
        }
    }
    return result;
}
