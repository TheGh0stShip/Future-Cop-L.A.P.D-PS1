#include "game/types.h"

extern s32 func_8001A074(s32 *vector);

s32 func_8001A13C(s32 *a, s32 *b)
{
    s32 delta[2];

    delta[0] = a[2] - b[2];
    delta[1] = a[0] - b[0];
    return func_8001A074(delta);
}
