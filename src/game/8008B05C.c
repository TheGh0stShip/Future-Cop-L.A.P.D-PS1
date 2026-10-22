#include "../../include/game/types.h"

struct BufferShape {
    u8 pad_00[0xE3];
    u8 width;
    u8 pad_E4[5];
    u8 height;
    u8 pad_EA[2];
    u8 *data;
};

u8 *func_8008B05C(struct BufferShape *shape)
{
    u32 width_bytes = ((shape->width + 1) >> 1) * 4;
    u32 height_bytes = ((shape->height * 5 + 3) & 0xFFC) + 4;

    return shape->data + width_bytes + height_bytes;
}
