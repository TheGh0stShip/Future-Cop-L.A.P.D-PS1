#include "game/types.h"

struct FontGlyph {
    u8 unk00[8];
    s8 advance;
};

struct FontEntry {
    u8 unk00[6];
    s16 height;
};

extern struct FontEntry *D_8009F35C;

extern struct FontGlyph *func_800489C8(struct FontEntry *font, u8 character);

void func_800492A0(u8 *text, s32 *width, s32 *height)
{
    s32 total;
    s32 lineHeight;
    u8 character;

    lineHeight = total = 0;
    if (D_8009F35C != 0) {
        for (;;) {
            character = *text;
            if (character == 0) {
                break;
            }
            text++;
            if (character < 0x20) {
                continue;
            }
            total += func_800489C8(D_8009F35C, character)->advance;
        }
        lineHeight = D_8009F35C->height;
    }
    *height = lineHeight;
    *width = total;
}
