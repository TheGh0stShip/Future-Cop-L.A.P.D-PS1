#include "game/types.h"

struct Unk52D7CModel {
    u8 unk00[0x34];
    u8 *frames;
};

struct Unk52D7CPart {
    struct Unk52D7CModel *model;
    u8 matrix[0x30];
    u8 output[0x14];
    s16 frame;
    s16 unk4A;
};

struct Unk52D7CState {
    u8 unk00[0x178];
    struct Unk52D7CPart parts[2];
};

struct Unk52D7CInfo {
    u8 unk00[0xAC];
    struct Unk52D7CState *state;
};

struct Unk52D7C {
    u8 unk00[0x28];
    u8 coord[0x8C];
    struct Unk52D7CInfo *info;
};

extern void func_80052960(u8 *coord, u8 *first, u8 *second);
extern void func_80010750(u8 *matrix);
extern void func_80010234(u8 *frame, u8 *output, s32 count);

void func_80052D7C(struct Unk52D7C *object)
{
    struct Unk52D7CState *state;

    state = object->info->state;
    func_80052960(object->coord, state->parts[0].matrix, state->parts[1].matrix);
    func_80010750(state->parts[0].matrix);
    func_80010234(state->parts[0].model->frames + (state->parts[0].frame << 4) + 8,
                  state->parts[0].output, 1);
    func_80010750(state->parts[1].matrix);
    func_80010234(state->parts[1].model->frames + (state->parts[1].frame << 4) + 8,
                  state->parts[1].output, 1);
}
