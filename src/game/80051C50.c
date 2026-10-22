#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk51C50View {
    s32 unk00[3];
    struct Vec3s32 up;
    struct Vec3s32 target;
};

struct Unk51C50 {
    u8 unk00[0xF0];
    s16 angle;
};

extern void func_80010974(struct Vec3s32 *up, struct Vec3s32 *target, struct Unk51C50View *view);
extern void func_800188D0(s32 arg0, s32 arg1, s32 angle, struct Unk51C50View *view);

void func_80051C50(struct Unk51C50 *object, s32 arg1, struct Vec3s32 *target,
                   struct Unk51C50View *view)
{
    if (object->angle == 0) {
        view->target = *target;
        view->up.x = 0;
        view->up.y = 0x10000;
        view->up.z = 0;
        func_80010974(&view->up, &view->target, view);
    } else {
        func_800188D0(arg1, 0, object->angle, view);
    }
}
