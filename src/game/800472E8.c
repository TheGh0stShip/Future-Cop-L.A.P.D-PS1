#include "game/types.h"

struct SoundBank {
    s32 samples[4];
    u8 count;
    u8 cursor;
    u8 voices;
    u8 unk13;
    u8 used;
    u8 unk15;
    u8 key;
    u8 unk17;
    s32 head;
};

extern struct SoundBank D_800A7938[];

s32 func_800472E8(s32 key)
{
    s32 i;

    for (i = 0; i < 0x6E; i++) {
        if (D_800A7938[i].used == 0) {
            i = -1;
            break;
        }
        if (D_800A7938[i].key == key) {
            break;
        }
    }
    if (i == 0x6E) {
        i = -1;
    }
    return i;
}
