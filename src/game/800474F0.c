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
    s32 result;

    result = 0;
    if (handle == 0) {
        goto set_result;
    }
    if (handle->resource == 0) {
        goto done;
    }
    if (handle->resource->serial != handle->serial) {
        goto done;
    }
    func_80046A34(handle->resource);
    handle->resource = 0;
    result = 1;
set_result:
    return result;
done:
    return result;
}
