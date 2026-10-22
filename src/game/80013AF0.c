#include "game/types.h"

struct ModelFrame {
    u16 unk00;
    s16 unk02;
    s16 radius;
    u16 unk06;
    u8 unk08[8];
};

struct ModelFrames {
    u8 header[0x10];
    struct ModelFrame frames[1];
};

struct Model {
    u8 unk00[0x0C];
    u32 flags;
    u8 unk10[0x24];
    struct ModelFrames *frames;
};

struct ModelInstance {
    struct Model *model;
    u8 coord[0x3C];
    s32 radius;
    u16 unk44;
    u16 unk46;
    s16 frame;
    u8 flags;
};

void func_80013AF0(struct ModelInstance *instance, struct Model *model, u32 options)
{
    instance->model = model;
    if (model == 0) {
        instance->radius = 0;
        instance->unk44 = 0;
        instance->unk46 = 0;
    } else {
        instance->radius = model->frames->frames[0].radius << 4;
        instance->unk44 = instance->model->frames->frames[0].unk00;
        instance->unk46 = instance->model->frames->frames[0].unk06;
    }
    instance->frame = 0;
    instance->flags = 0;
    if (options & 0x80000) {
        instance->flags = 0x10;
    }
    if (options & 0x8000) {
        instance->flags |= 8;
    }
    if (options & 0x10000) {
        instance->flags |= 4;
    }
    if (model != 0 && ((model->flags >> 30) & 1)) {
        instance->flags |= 0x40;
    }
}
