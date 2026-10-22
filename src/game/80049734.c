#include "game/types.h"

struct Unk49734Data {
    s32 unk00;
    s32 unk04;
    s32 count;
};

struct Unk49734Resource {
    struct Unk49734Data *data;
    s32 unk04;
    s32 unk08;
};

extern s32 *D_8009F778;
extern s32 D_8009F788;

extern u8 func_80019AA8(struct Unk49734Resource *resource);
extern void func_80019A74(struct Unk49734Resource *resource);
extern void func_80019D00(struct Unk49734Resource *resource);

void func_80049734(struct Unk49734Resource *resource)
{
    if (func_80019AA8(resource) == 0) {
        resource->unk08 = 0;
        func_80019A74(resource);
        D_8009F778 = &resource->data->count;
        D_8009F788 = resource->data->count;
    } else {
        func_80019D00(resource);
    }
}
