#include "game/types.h"

struct LineF2 {
    u32 tag;
    u32 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
};

extern u8 *D_8009FA90;
extern u8 *D_8009F9C0;

extern void func_8001846C(s32 id, s32 *first, s32 *second);

void func_8005A740(s32 origin, s32 id)
{
    struct LineF2 *line;
    s32 first;
    s32 second;

    func_8001846C(id, &first, &second);
    line = (struct LineF2 *)D_8009FA90;
    if (D_8009FA90 < D_8009F9C0) {
        D_8009FA90 += 0x1C;
    }
    line->tag = 0x3000000;
    line->code = 0x40808080;
    line->x0 = (origin + second * 24) >> 16;
    line->y0 = (first * 24 + 0x2C0000) >> 16;
    line->x1 = (origin + second * 30) >> 16;
    line->y1 = (first * 30 + 0x2C0000) >> 16;
}
