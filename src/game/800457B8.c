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

extern void func_80046698(s32 handle);
extern void func_80045548(struct SoundChannel *channel);

void func_800457B8(s32 arg0, s32 index)
{
    struct SoundChannel *channel;

    if (index < 0) {
        return;
    }
    channel = &D_800A1160[index];
    if (channel->id == -1) {
        return;
    }
    channel->id = -1;
    if (channel->unk28 != 0) {
        func_80046698(channel->unk28);
    }
    func_80045548(channel);
}
