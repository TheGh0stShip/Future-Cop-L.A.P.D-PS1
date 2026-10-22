#include "game/types.h"

struct StreamChannel {
    s32 id;
    s32 voice;
    s32 left;
    s32 right;
    s32 start;
    s32 length;
    u8 looping;
    u8 unk19[0x0B];
};

struct StreamState {
    u8 unk00[0x24];
    struct StreamChannel channels[3];
};

extern struct StreamState D_800A0968;

void func_80022B28(s32 start, s32 length, s32 channel, u8 looping)
{
    if (looping) {
        D_800A0968.channels[channel].looping = 1;
    }
    D_800A0968.channels[channel].length = length;
    D_800A0968.channels[channel].start = start;
}
