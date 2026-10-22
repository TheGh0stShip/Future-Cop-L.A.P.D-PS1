#include "game/types.h"

struct Unk4749CResource {
    u8 unk00[0x10];
    s32 serial;
};

struct Unk4749CHandle {
    struct Unk4749CResource *resource;
    s32 serial;
};

extern void func_80046A34(struct Unk4749CResource *resource);

s32 func_800474F0(struct Unk4749CHandle *handle)
{
    if (handle == 0) {
        return 0;
    }
    if (handle->resource == 0) {
        return 0;
    }
    if (handle->resource->serial != handle->serial) {
        return 0;
    }
    func_80046A34(handle->resource);
    handle->resource = 0;
    return 1;
}
