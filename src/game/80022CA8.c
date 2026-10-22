#include "game/types.h"

struct StreamChannel {
    s32 id;
    s32 voice;
    s32 left;
    s32 right;
    u8 unk10[0x14];
};

struct StreamState {
    u8 flag;
    u8 unk01[3];
    s32 active;
    s32 unk08;
    s32 stopping;
    s16 unk10;
    u8 unk12[0x0E];
    s32 unk20;
    struct StreamChannel channels[3];
    u8 unk90[0];
};

struct StreamTail {
    u8 unk00[0x70];
    s32 rate;
    s32 currentRate;
    s32 unk78;
    s32 unk7C;
    s32 ready;
    s32 unk84;
    s32 deadline;
};

extern struct StreamState D_800A0968;

void func_80022CA8(void)
{
    struct StreamTail *tail;
    u32 i;

    tail = (struct StreamTail *)&D_800A0968;
    D_800A0968.flag = 0;
    D_800A0968.active = 0;
    tail->deadline = 0;
    D_800A0968.unk08 = 0;
    D_800A0968.stopping = 0;
    D_800A0968.unk20 = 0;
    D_800A0968.unk10 = 0;
    tail->unk84 = 0;
    tail->unk78 = 0;
    tail->unk7C = 0;
    tail->ready = 1;
    tail->currentRate = tail->rate;
    for (i = 0; i < 2; i++) {
        D_800A0968.channels[i].left = 0;
        D_800A0968.channels[i].right = 0x40;
        D_800A0968.channels[i].id = -1;
    }
}
