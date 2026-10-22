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
extern struct SoundChannel *D_8009F724;
extern struct SoundChannel *D_8009F728;
extern struct SoundChannel *D_8009F72C;

extern void func_8004582C(void);

void func_80045548(struct SoundChannel *channel)
{
    if (channel->prev != 0) {
        channel->prev->next = channel->next;
    } else {
        D_8009F728 = channel->next;
    }
    if (channel->next != 0) {
        channel->next->prev = channel->prev;
    } else {
        D_8009F72C = channel->prev;
    }
    channel->next = D_8009F724;
    D_8009F724 = channel;
    if (D_8009F720 != 0) {
        D_8009F720--;
    } else {
        func_8004582C();
    }
}
