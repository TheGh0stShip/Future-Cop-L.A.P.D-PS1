#include "game/types.h"

struct RotationMatrix {
    s32 values[9];
};

struct RotationPoint {
    s16 x;
    s16 y;
    s16 z;
    s16 pad;
};

void func_8001B148(struct RotationMatrix *matrix, struct RotationPoint *source,
                   struct RotationPoint *destination, s32 count)
{
    s32 m0;
    s32 m1;
    s32 m2;
    s32 m3;
    s32 m4;
    s32 m5;
    s32 m6;
    s32 m7;
    s32 m8;

    if (count <= 0) {
        return;
    }

    m0 = matrix->values[0] >> 4;
    m1 = matrix->values[1] >> 4;
    m2 = matrix->values[2] >> 4;
    m3 = matrix->values[3] >> 4;
    m4 = matrix->values[4] >> 4;
    m5 = matrix->values[5] >> 4;
    m6 = matrix->values[6] >> 4;
    m7 = matrix->values[7] >> 4;
    m8 = matrix->values[8] >> 4;

    do {
        s32 x;
        s32 y;
        s32 z;

        x = source->x;
        y = source->y;
        z = source->z;
        destination->x = (x * m0 + y * m3 + z * m6) >> 12;
        destination->y = (x * m1 + y * m4 + z * m7) >> 12;
        destination->z = (x * m2 + y * m5 + z * m8) >> 12;
        source++;
        destination++;
        count--;
    } while (count != 0);
}
