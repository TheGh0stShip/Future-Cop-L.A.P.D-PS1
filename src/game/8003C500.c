#include "game/types.h"

struct Unk3C500Cell {
    u8 unk00[0x14];
    u8 id;
    u8 next;
};

extern struct Unk3C500Cell *D_800A7040[][0x100];
extern void func_8003C4C4(u8 column, s32 row, u8 bits);

void func_8003C500(u8 column, u8 stop, s32 row)
{
    struct Unk3C500Cell **line;
    struct Unk3C500Cell *cell;

    line = D_800A7040[row];
    do {
        cell = line[column];
        column = cell->next;
        func_8003C4C4(column, row, 4);
    } while (cell->id != stop);
}
