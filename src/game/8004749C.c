#include "game/types.h"

struct Unk4749CResource {
    u8 unk00[0x10];
    s32 serial;
};

struct Unk4749CHandle {
    struct Unk4749CResource *resource;
    s32 serial;
};

void func_8004749C(struct Unk4749CHandle *handle, struct Unk4749CResource *resource)
{
    if (resource != 0 && handle != 0) {
        handle->resource = resource;
        handle->serial = resource->serial;
    }
}
