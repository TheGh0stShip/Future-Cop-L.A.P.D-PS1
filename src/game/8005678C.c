#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk5678COffset {
    s16 angle;
    s16 pitch;
    s32 distance;
    u8 data[0x0C];
};

struct Unk5678CInfo {
    u8 unk00[0xA8];
    s16 unkA8;
    u8 unkAA[0x12];
    s32 unkBC;
};

struct Unk5678C {
    u8 unk00[0x5C];
    struct Vec3s32 position;
    u8 unk68[0x44];
    struct Unk5678CInfo *info;
    s16 heading;
};

extern struct Unk5678COffset D_8009AD08[];

extern void func_8005612C(struct Vec3s32 *position, s32 heading, s32 angle, s32 pitch,
                          s32 distance, u8 *data, s32 *out, s32 arg7, s32 arg8);
extern void func_80017860(s32 angle, s32 pitch, s32 arg2);
extern void func_800177AC(s32 *out);

void func_8005678C(struct Unk5678C *object, s32 index, s32 *out, s32 apply)
{
    struct Vec3s32 position;
    struct Unk5678COffset *offset;
    s32 heading;

    heading = object->heading;
    position = object->position;
    offset = &D_8009AD08[index];
    func_8005612C(&position, heading, heading + offset->angle, offset->pitch,
                  offset->distance, offset->data, out, object->info->unkA8,
                  object->info->unkBC);
    if (apply) {
        func_80017860(object->heading + offset->angle, offset->pitch, 0);
        func_800177AC(out);
    }
}
