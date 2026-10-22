#include "game/types.h"

struct Matrix32 {
    s32 m[3][3];
};

void func_80019470(struct Matrix32 *source, struct Matrix32 *destination)
{
    destination->m[0][0] = source->m[0][0];
    destination->m[0][1] = source->m[1][0];
    destination->m[0][2] = source->m[2][0];
    destination->m[1][0] = source->m[0][1];
    destination->m[1][1] = source->m[1][1];
    destination->m[1][2] = source->m[2][1];
    destination->m[2][0] = source->m[0][2];
    destination->m[2][1] = source->m[1][2];
    destination->m[2][2] = source->m[2][2];
}
