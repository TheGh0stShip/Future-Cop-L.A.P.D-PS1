#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Camera {
    struct Vec3s32 position;
    u8 unk0C[0x1C];
    u8 matrix[0x24];
    struct Vec3s32 inverse;
    u8 unk58[0x24];
    struct Vec3s32 eye;
};

extern struct Camera *D_8009F9E4;

extern void func_8001A81C(u8 *matrix, struct Vec3s32 *input, struct Vec3s32 *output);

void func_800177AC(struct Vec3s32 *position)
{
    struct Vec3s32 rotated;

    D_8009F9E4->position = *position;
    D_8009F9E4->eye = *position;
    func_8001A81C(D_8009F9E4->matrix, position, &rotated);
    rotated.x = -rotated.x;
    rotated.y = -rotated.y;
    rotated.z = -rotated.z;
    D_8009F9E4->inverse = rotated;
}
