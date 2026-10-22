#include "game/types.h"

struct HalfRect {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
};

extern void func_8001C148(u8 red, u8 green, u8 blue, struct HalfRect *rect, u8 blend);

void func_8001C214(u32 color, s32 x, s32 y, s32 w, s32 h, u8 blend)
{
    struct HalfRect rect;

    rect.x = x >> 1;
    rect.y = y >> 1;
    rect.w = w >> 1;
    rect.h = h >> 1;
    func_8001C148((color >> 8) & 0x7C, (color >> 3) & 0x7C, (color & 0x1F) << 2, &rect, blend);
}
