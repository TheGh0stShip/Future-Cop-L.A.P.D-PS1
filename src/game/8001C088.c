#include "game/types.h"

struct Unk1C040 {
    u8 unk00[0x18];
    u8 enabled;
    u8 red;
    u8 green;
    u8 blue;
    u8 unk1C[0x40];
};

extern s8 D_8009F224;
extern u8 D_8009F225;
extern struct Unk1C040 D_800A06E0[];

extern void func_800932F0(struct Unk1C040 *entry);

void func_8001C088(u8 mode, u8 enable)
{
    s32 i;

    D_8009F224 = mode;
    if (enable) {
        for (i = 0; i < D_8009F225; i++) {
            D_800A06E0[i].enabled = 1;
        }
    } else {
        for (i = 0; i < D_8009F225; i++) {
            D_800A06E0[i].enabled = 0;
        }
    }
    func_800932F0(&D_800A06E0[D_8009F224]);
}
