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
    register struct Unk4749CResource *resource __asm__("$3");
    register s32 result __asm__("$2");

    resource = handle->resource;
    result = 0;
    if (resource != 0) {
        result = resource->serial == handle->serial;
    }
    return result;
}
