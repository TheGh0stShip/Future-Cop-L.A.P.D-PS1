#include "game/types.h"

struct FontGlyph {
    u8 unk00[8];
    s8 advance;
};

struct FontEntry;

extern struct FontEntry *D_8009F35C;

extern struct FontGlyph *func_800489C8(struct FontEntry *font, u8 character);

s32 func_80049340(u8 *text)
{
    struct FontEntry *font;
    s32 total;
    u8 character;

    font = D_8009F35C;
    total = 0;
    if (font != 0) {
        while ((character = *text++) != 0) {
            if (character >= 0x20) {
                total += func_800489C8(font, character)->advance;
            }
        }
    }
    return total * 2;
}
