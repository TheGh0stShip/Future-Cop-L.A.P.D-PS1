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
extern s32 D_8009F720;
extern struct SoundChannel *D_8009F724;
extern struct SoundChannel *D_8009F728;
extern struct SoundChannel *D_8009F72C;

void func_8004582C(void)
{
    struct SoundChannel *channel;
    s32 i;

    channel = D_800A1160;
    for (i = 0; i < 0x1E; i++) {
        channel->id = -1;
        channel->index = i;
        channel->next = channel + 1;
        channel++;
    }
    channel[-1].next = 0;
    D_8009F720 = 0;
    D_8009F724 = D_800A1160;
    D_8009F728 = 0;
    D_8009F72C = 0;
}
