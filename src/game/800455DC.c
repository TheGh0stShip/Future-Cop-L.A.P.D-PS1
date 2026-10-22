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

extern s32 D_8009F720;
extern struct SoundChannel *D_8009F728;
extern struct SoundChannel *D_8009F72C;

void func_800455DC(struct SoundChannel *channel)
{
    struct SoundChannel *prev;
    struct SoundChannel *next;

    next = 0;
    if (channel->priority == 0) {
        prev = D_8009F72C;
        channel->prev = prev;
    } else {
        prev = 0;
        for (next = D_8009F728; next != 0; next = next->next) {
            if (next->priority < channel->priority) {
                break;
            }
            prev = next;
        }
        channel->prev = prev;
    }
    channel->next = next;
    if (prev != 0) {
        prev->next = channel;
    } else {
        D_8009F728 = channel;
    }
    if (next != 0) {
        next->prev = channel;
    } else {
        D_8009F72C = channel;
    }
    D_8009F720++;
}
