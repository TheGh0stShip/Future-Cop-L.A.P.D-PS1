#include "game/types.h"

extern s32 D_8009FA0C;

s32 func_80019E70(s16 *point, s32 margin)
{
    s32 depth;
    s32 limit;
    s32 extent;

    depth = point[2] << 4;
    if (depth + margin < 0x406) {
        return 1;
    }
    if (D_8009FA0C == 1) {
        limit = depth + ((point[2] << 6) + (margin << 4) + ((margin + margin * 2) >> 2));
        extent = point[0] << 8;
    } else {
        limit = depth + ((point[2] << 6) + (margin << 5) + ((margin + margin * 2) >> 3));
        extent = point[0] << 9;
    }
    if (extent < 0) {
        extent = -extent;
    }
    if (limit < extent) {
        return 0;
    }
    extent = point[1] << 10;
    if (extent < 0) {
        extent = -extent;
    }
    limit = (depth << 4) - depth + (margin << 6) + (((margin * 8 - margin) * 16 - margin) >> 6);
    return (limit >= extent) << 1;
}
