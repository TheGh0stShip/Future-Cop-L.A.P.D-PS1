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
    u8 unk00[0x34];
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

void func_800175F8(struct ModelInstance *instance, s32 frame)
{
    if (instance->frame == frame) {
        return;
    }
    instance->frame = frame;
    instance->radius = instance->model->frames->frames[frame].radius << 4;
    instance->unk44 = instance->model->frames->frames[frame].unk00;
    instance->unk46 = instance->model->frames->frames[frame].unk06;
}
