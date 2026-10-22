#include "game/types.h"

typedef struct {
    u8 iq_y[64];
    u8 iq_c[64];
    s16 dct[64];
} DecDctEnv;

extern s32 D_8009E8B4[];
extern s32 D_8009E8F4[];
extern s32 D_8009E938[];

DecDctEnv *func_80095EC4(DecDctEnv *environment)
{
    s32 i;
    s32 *source;
    s32 *destination;

    destination = (s32 *)environment->iq_y;
    source = D_8009E8B4;
    for (i = 15; i != -1; i--) {
        *destination++ = *source++;
    }

    destination = (s32 *)environment->iq_c;
    source = D_8009E8F4;
    for (i = 15; i != -1; i--) {
        *destination++ = *source++;
    }

    destination = (s32 *)environment->dct;
    source = D_8009E938;
    for (i = 31; i != -1; i--) {
        *destination++ = *source++;
    }

    return environment;
}
