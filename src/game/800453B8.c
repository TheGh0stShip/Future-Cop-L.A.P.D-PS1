#include "game/types.h"

struct SoundVoice {
    u8 unk00[0x0D];
    u8 index;
    u8 unk0E[2];
    s32 unk10;
    u8 unk14[8];
    struct SoundVoice *next;
    u8 unk20[0x0C];
};

struct Unk453B8Bank {
    s32 unk00;
    u8 unk04[0x10];
};

struct Unk453B8Stream {
    s32 unk00;
    s32 unk04;
    s32 unk08;
};

extern struct SoundVoice D_800AACC8[];
extern struct Unk453B8Bank D_800A10E0[];
extern struct Unk453B8Stream D_800A1810[];
extern struct SoundVoice *D_8009F73C;
extern s32 D_8009F740;
extern struct SoundVoice *D_8009F744;
extern struct SoundVoice *D_8009F748;
extern struct SoundVoice *D_8009F74C;
extern struct SoundVoice *D_8009F750;
extern struct SoundVoice *D_8009F754;
extern struct SoundVoice *D_8009F758;
extern s32 D_8009F32C;
extern u8 D_8009F738;

extern void func_80044EF8(void);
extern void func_8004582C(void);
extern s32 func_80046AD8(void);

void func_800453B8(void)
{
    u32 i;
    struct SoundVoice *voice;

    voice = D_800AACC8;
    D_8009F73C = voice;
    D_8009F740 = 0x18;
    D_8009F744 = 0;
    D_8009F748 = 0;
    D_8009F74C = 0;
    D_8009F750 = 0;
    D_8009F754 = 0;
    D_8009F758 = 0;
    for (i = 0; i < 0x18; i++) {
        voice->index = i;
        voice->next = voice + 1;
        voice->unk10 = 0;
        voice++;
    }
    voice[-1].next = 0;
    func_80044EF8();
    func_8004582C();
    for (i = 0; i < 6; i++) {
        D_800A10E0[i].unk00 = 0;
    }
    D_8009F32C = 0;
    for (i = 0; i < 2; i++) {
        D_800A1810[i].unk08 = func_80046AD8();
        D_800A1810[i].unk00 = 0;
    }
    D_8009F738 = 0;
}
