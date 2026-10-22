#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Camera {
    u8 unk00[0x28];
    u8 matrix[0x54];
    struct Vec3s32 eye;
};

struct Unk178DC {
    u8 unk00[0x24];
    struct Vec3s32 translation;
};

extern struct Camera *D_8009F9E4;

extern void func_80010820(u8 *matrix);
extern void func_8001006C(struct Unk178DC *object, s32 arg1, s32 count);

void func_800178DC(struct Unk178DC *object, s32 arg1)
{
    struct Vec3s32 saved;
    struct Camera *camera;

    saved = object->translation;
    camera = D_8009F9E4;
    object->translation.x -= camera->eye.x;
    object->translation.y -= camera->eye.y;
    object->translation.z -= camera->eye.z;
    func_80010820(camera->matrix);
    func_8001006C(object, arg1, 4);
    object->translation = saved;
}
