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

void func_8004572C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 index)
{
    if (index >= 0) {
        D_800A1160[index].unk0C = arg1;
        D_800A1160[index].unk10 = arg2;
    }
}
