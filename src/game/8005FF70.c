#include "game/types.h"

struct Unk5FF70 {
    u8 key;
    u8 unk01[0x2F];
};

extern s32 D_8009F7F8;
extern struct Unk5FF70 D_800A2300[];

s32 func_8005FF70(s32 key)
{
    s32 i;

    for (i = 0; i < D_8009F7F8; i++) {
        if (D_800A2300[i].key == key) {
            return i;
        }
    }
    return -1;
}
