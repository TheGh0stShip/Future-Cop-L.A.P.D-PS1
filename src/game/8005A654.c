#include "game/types.h"

struct Unk5A654Sample {
    s16 id;
    u8 rate;
};

extern struct Unk5A654Sample **D_8009F7D8;
extern s32 D_8009F7DC;

s32 func_8005A654(s32 id)
{
    s32 i;

    for (i = 0; i < D_8009F7DC; i++) {
        if (D_8009F7D8[i]->id == id) {
            return i;
        }
    }
    return -1;
}
