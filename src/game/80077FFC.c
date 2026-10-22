#include "game/types.h"

struct Unk77FFC {
    u8 unk00[0x1F];
    u8 state;
    u8 unk20[4];
};

extern struct Unk77FFC D_8009B4E4[];
extern s8 D_8009B72C[];
extern u8 D_8009F3C2;

void func_80077FFC(void)
{
    if (D_8009B4E4[D_8009B72C[0]].state == 10 &&
        D_8009B4E4[D_8009B72C[1]].state == D_8009B4E4[D_8009B72C[0]].state &&
        D_8009B4E4[D_8009B72C[2]].state == D_8009B4E4[D_8009B72C[1]].state &&
        D_8009B4E4[D_8009B72C[3]].state == D_8009B4E4[D_8009B72C[2]].state) {
        D_8009F3C2 = 1;
    } else {
        D_8009F3C2 = 0;
    }
}
