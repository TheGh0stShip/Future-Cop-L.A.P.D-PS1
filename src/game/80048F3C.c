#include "game/types.h"

struct FontEntry {
    u8 unk00[0x18];
    void *glyphs;
    u8 unk1C[0x100];
};

extern struct FontEntry D_800A19A0[];
extern s32 D_8009F364;

extern void func_800181D4(void *block);

void func_80048F3C(void)
{
    s32 i;

    for (i = 0; i < D_8009F364; i++) {
        func_800181D4(D_800A19A0[i].glyphs);
    }
}
