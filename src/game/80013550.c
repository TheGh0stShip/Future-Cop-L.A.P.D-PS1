#include "game/types.h"

struct Unk13550 {
    void *data;
    s32 unk04;
    s32 unk08;
    u8 unk0C[0x10];
    s32 unk1C;
    s32 unk20;
};

extern u8 func_80019AA8(struct Unk13550 *resource);
extern void func_800131A0(void **out, void *data, s32 arg2, s32 arg3);
extern void func_80019A74(struct Unk13550 *resource);
extern void func_80019D00(struct Unk13550 *resource);

void func_80013550(struct Unk13550 *resource)
{
    void *loaded;

    loaded = 0;
    if (func_80019AA8(resource) == 0) {
        func_800131A0(&loaded, resource->data, resource->unk20, resource->unk1C);
        resource->unk08 = 0;
        resource->data = loaded;
        func_80019A74(resource);
    } else {
        func_80019D00(resource);
    }
}
