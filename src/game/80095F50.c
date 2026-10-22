#include "game/types.h"

typedef struct {
    u8 iq_y[64];
    u8 iq_c[64];
    s16 dct[64];
} DecDctEnv;

extern s32 D_8009E8B0[];
extern s32 D_8009E8B4[];
extern s32 D_8009E8F4[];
extern s32 D_8009E934[];
extern void func_80096240(s32 *data, u32 size);

DecDctEnv *func_80095F50(DecDctEnv *environment)
{
    s32 i;
    s32 *source;
    s32 *destination;

    destination = D_8009E8B4;
    source = (s32 *)environment->iq_y;
    for (i = 15; i != -1; i--) {
        *destination++ = *source++;
    }

    destination = D_8009E8F4;
    source = (s32 *)environment->iq_c;
    for (i = 15; i != -1; i--) {
        *destination++ = *source++;
    }

    func_80096240(D_8009E8B0, 0x20);
    func_80096240(D_8009E934, 0x20);

    return environment;
}
