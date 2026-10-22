#include "game/types.h"

struct Unk1BFE4 {
    u8 data[0x14];
};

extern s8 D_8009F224;
extern u8 D_8009F226;
extern struct Unk1BFE4 D_800A06B0[];

extern void func_800934BC(struct Unk1BFE4 *entry);

s32 func_8001BFE4(void)
{
    if (D_8009F224 >= 0) {
        D_8009F226 = D_8009F224;
        func_800934BC(&D_800A06B0[D_8009F224]);
        return 0;
    }
    return 0xF;
}
