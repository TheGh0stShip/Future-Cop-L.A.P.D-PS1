#include "game/types.h"

struct Unk631A0 {
    s32 key;
    u8 value;
};

extern s32 D_8009F838;
extern struct Unk631A0 D_800A31A0[];

void func_80063194(s32 key)
{
    s32 i;

    for (i = 0; i < D_8009F838; i++) {
        if (D_800A31A0[i].key == key) {
            D_8009F838--;
            if (i < D_8009F838) {
                D_800A31A0[i] = D_800A31A0[D_8009F838];
            } else {
                D_800A31A0[i].key = 0;
            }
            return;
        }
    }
}
