#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk752E0Light {
    u8 unk00[9];
    u8 red;
    u8 green;
    u8 blue;
};

struct Unk752E0Request {
    s32 unk00;
    s32 unk04;
    struct Vec3s32 position;
    u8 kind;
    u8 slot;
    s16 height;
    s16 width;
    s16 depth;
    s16 angle;
    s16 unk1E;
    s16 unk20;
    s8 unk22;
    u8 red;
    u8 green;
    u8 blue;
    u8 unk26[2];
    s16 scale;
    u8 unk2A[6];
};

struct Unk752E0 {
    s32 unk00;
    struct Vec3s32 position;
    u16 size;
    u8 unk12[3];
    u8 red;
    u8 green;
    u8 blue;
    u8 unk18[2];
    s16 angle;
    u8 unk1C[8];
    struct Unk752E0Light *light;
};

extern u32 func_80019668(void);
extern s32 func_8005A6A4(s32 id, struct Unk752E0Request *request,
                         struct Unk752E0Light **result);

void func_800752E0(struct Unk752E0 *object)
{
    struct Unk752E0Request request;

    request.position = object->position;
    request.kind = 0x16;
    request.height = request.position.y >> 4;
    request.angle = object->angle = func_80019668() & 0xFFF;
    request.unk22 = -1;
    request.unk1E = 0;
    request.scale = -1;
    request.unk20 = 0;
    request.width = request.depth = object->size << 1;
    request.red = object->red;
    request.green = object->green;
    request.blue = object->blue;
    func_8005A6A4(0xA, &request, &object->light);
    if (object->light != 0) {
        object->light->red = object->red;
        object->light->green = object->green;
        object->light->blue = object->blue;
    }
}
