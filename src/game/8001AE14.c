#include "game/types.h"

void func_8001AE14(s32 *matrix, s32 *vector, s32 *out)
{
    s32 x;
    s32 y;
    s32 z;

    x = vector[0] >> 2;
    y = vector[1] >> 2;
    z = vector[2] >> 2;
    out[0] = (x * (matrix[0] >> 2) + y * (matrix[1] >> 2) + z * (matrix[2] >> 2)) >> 12;
    out[1] = (x * (matrix[3] >> 2) + y * (matrix[4] >> 2) + z * (matrix[5] >> 2)) >> 12;
    out[2] = (x * (matrix[6] >> 2) + y * (matrix[7] >> 2) + z * (matrix[8] >> 2)) >> 12;
}
