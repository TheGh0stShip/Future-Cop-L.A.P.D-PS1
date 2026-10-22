#include "game/types.h"

struct Unk17508 {
    void *model;
};

extern u8 func_80013F08(struct Unk17508 *instance);
extern void func_80016A64(struct Unk17508 *instance, s32 mode, s32 arg2);

u8 func_80017508(struct Unk17508 *instance, u32 *flags, s32 arg2)
{
    u8 visible;

    if (instance->model == 0 || (*flags & 0x20)) {
        return 0;
    }
    visible = func_80013F08(instance);
    if (visible) {
        func_80016A64(instance, (*flags >> 25) & 1, arg2);
    }
    return visible;
}
