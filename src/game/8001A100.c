#include "game/types.h"

extern s32 func_8001A074(s32 *vector);

s32 func_8001A100(s32 *vector)
{
    s32 planar[2];

    planar[0] = func_8001A074(vector);
    planar[1] = vector[2];
    return func_8001A074(planar);
}
