#include "../../include/game/types.h"

struct BufferShape {
    u8 pad_00[0xE3];
    u8 width;
    u8 pad_E4[5];
    u8 height;
    u8 pad_EA[2];
    u32 data;
};

u32 func_8008B05C(struct BufferShape *shape)
{
    register s32 width __asm__("$2");
    register u32 height __asm__("$3");
    register u32 data __asm__("$4");

    width = shape->width;
    height = shape->height;
    data = shape->data;
    width = ((width + 1) >> 1) * 4;
    height = (height * 5 + 3) & 0xFFC;
    height += 4;
    return width + height + data;
}
