#include "game/types.h"

struct Unk4749CResource {
    u8 unk00[0x10];
    s32 serial;
};

struct Unk4749CHandle {
    struct Unk4749CResource *resource;
    s32 serial;
};

s32 func_800474C4(struct Unk4749CHandle *handle)
{
    if (handle->resource == 0) {
        return 0;
    }
    return handle->resource->serial == handle->serial;
}
