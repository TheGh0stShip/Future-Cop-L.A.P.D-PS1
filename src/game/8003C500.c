#include "game/types.h"

struct Unk3C500Cell {
    u8 unk00[0x14];
    u8 id;
    u8 next;
};

extern struct Unk3C500Cell *D_800A7040[][0x100];
extern void func_8003C4C4(u8 column, s32 row, u8 bits);

void func_8003C500(s32 column, s32 stop, s32 row)
{
    struct Unk3C500Cell **line;
    struct Unk3C500Cell **base;
    struct Unk3C500Cell *cell;
    register s32 rowValue __asm__("$18");
    register s32 columnValue __asm__("$17");
    register s32 stopValue __asm__("$20");

    rowValue = row;
    columnValue = column;
    stopValue = stop;
    line = (base = D_800A7040[0], rowValue * 0x100 + base);
    do {
        cell = line[(u8)columnValue];
        __asm__("" : : "r"(columnValue));
        columnValue = cell->next;
        func_8003C4C4(columnValue, rowValue, 4);
    } while (cell->id != (u8)stopValue);
}
