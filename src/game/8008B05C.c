#include "../../include/game/types.h"

struct BufferShape {
    u8 pad_00[0xE3];
    u8 width;
    u8 pad_E4[5];
    u8 height;
    u8 pad_EA[2];
    u32 data;
};

s32 func_8008B05C(struct BufferShape *shape)
{
    s32 width;
    s32 height;
    s32 response_bytes;
    s32 base;
    s32 raw_width;
    s32 raw_height;

    raw_width = shape->width;
    raw_height = shape->height;
    base = shape->data;

    width = raw_width + 1;
    width >>= 1;
    width <<= 2;

    response_bytes = (raw_height << 2) + raw_height;
    height = (response_bytes + 3) & 0xFFC;
    height += 4;

    width += height;
    return width + base;
}
