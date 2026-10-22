#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk5920CModel {
    u8 unk00[0x10];
    u8 *animation;
};

struct Unk5920CType {
    u8 unk00[0xE0];
    s32 unkE0;
};

struct Unk5920CDef {
    u8 unk00[8];
    struct Vec3s32 position;
    u8 unk14[0x10];
    struct Unk5920CType *type;
};

struct Unk5920CSpawn {
    u8 unk00[8];
    struct Unk5920CDef *def;
};

struct Unk5920C {
    u8 unk00[0x24];
    u32 flags;
    struct Unk5920CModel *model;
    u8 unk2C[0x24];
    struct Vec3s32 position;
    u8 unk5C[0x16];
    u8 flags72;
    u8 unk73;
    s32 unk74;
    u8 unk78[0x28];
    u8 track[0x14];
    struct Unk5920CType *type;
};

extern void func_8002AD58(s32 kind, struct Unk5920C *object, struct Unk5920CSpawn *spawn,
                          s32 arg3);
extern void func_80013BD8(struct Unk5920CModel **model, s32 arg1, s32 arg2);
extern void func_80044988(u8 *data, u8 *track, u8 forward, s32 index);

void func_8005920C(struct Unk5920C *object, struct Unk5920CSpawn *spawn, s32 arg2)
{
    struct Unk5920CDef *def;

    func_8002AD58(2, object, spawn, arg2);
    def = spawn->def;
    object->type = def->type;
    func_80013BD8(&object->model, object->type->unkE0, 0);
    object->flags72 |= 0x30;
    func_80044988(object->model->animation, object->track, 1, 0);
    object->unk74 = 0;
    object->position = def->position;
    object->flags = (object->flags & 0xBFFFFFFF) | 0xA20210;
}
