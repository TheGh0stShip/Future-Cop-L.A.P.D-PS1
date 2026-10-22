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

extern u8 D_800A1830[];
extern s32 D_8009F720;
extern struct SoundChannel *D_8009F724;

extern s32 func_80045494(s32 id, u8 *table, s32 limit);
extern void func_800455DC(struct SoundChannel *channel);

s32 func_80045678(s32 sound, s32 id, s32 arg2, s32 arg3)
{
    struct SoundChannel *channel;
    s32 index;
    s32 slot;

    index = -1;
    slot = func_80045494(sound, D_800A1830, 0x47);
    if (slot == 0xFF) {
        return index;
    }
    if (D_8009F720 >= 0x1E) {
        return index;
    }
    channel = D_8009F724;
    channel->id = id;
    channel->unk28 = 0;
    channel->slot = slot;
    channel->unk0C = arg2;
    channel->unk10 = arg3;
    channel->priority = 0;
    D_8009F724 = channel->next;
    index = channel->index;
    func_800455DC(channel);
    return index;
}
