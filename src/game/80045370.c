#include "game/types.h"

struct SoundVoice;

extern u8 D_8009F738;
extern struct SoundVoice *D_8009F744;
extern struct SoundVoice *D_8009F74C;
extern struct SoundVoice *D_8009F754;

extern void func_8004530C(struct SoundVoice *voice);

void func_80045370(void)
{
    if (D_8009F738 != 0) {
        D_8009F738 = 0;
        func_8004530C(D_8009F744);
        func_8004530C(D_8009F74C);
        func_8004530C(D_8009F754);
    }
}
