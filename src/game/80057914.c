#include "game/types.h"

struct Vec3s32 {
    s32 x;
    s32 y;
    s32 z;
};

struct Unk57914Args {
    s32 tag;
    u8 kind;
    u8 unk05[3];
    struct Vec3s32 position;
    u32 flags;
    u16 unk18;
    u16 unk1A;
    u8 unk1C;
    u8 unk1D;
    u8 unk1E;
    u8 unk1F;
    u8 unk20;
    u8 unk21;
    u8 unk22;
    u8 unk23;
    void *owner;
};

struct Unk57914Request {
    void (*handler)();
    s32 unk04;
    struct Unk57914Args *args;
    s32 unk0C;
};

struct Unk57914 {
    u8 unk00[0x14];
    u16 tag;
    u8 unk16;
    u8 unk17;
    u8 unk18;
    u8 unk19;
    u16 unk1A;
    u16 unk1C;
    u8 unk1E[6];
    u32 flags;
    u8 coord[0x6A];
    u8 unk92;
    u8 unk93;
};

extern void D_80059ABC();

extern void func_80017668(void *source, s32 arg1, struct Vec3s32 *out);
extern void *func_8004D444(struct Unk57914Request *request);

void func_80057914(struct Unk57914 *object)
{
    struct Unk57914Request request;
    struct Unk57914Args args;

    request.handler = D_80059ABC;
    request.args = &args;
    request.unk04 = 0;
    request.unk0C = 0;
    args.kind = 0x12;
    args.tag = object->tag;
    args.unk1D = object->unk17;
    args.unk1C = object->unk18;
    args.flags = object->flags;
    args.unk18 = object->unk1C;
    args.unk1A = object->unk1A;
    func_80017668(object->coord, 0, &args.position);
    args.owner = object;
    args.unk20 = 0;
    args.unk21 = 0;
    args.unk1F = 0;
    args.unk22 = object->unk92;
    args.unk23 = object->unk93;
    func_8004D444(&request);
}
