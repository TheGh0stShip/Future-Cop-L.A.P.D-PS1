#include "game/types.h"

extern char D_8009F368[];

extern s32 func_8008CD30(char *buffer, char *format, s32 value);
extern void func_80049100(char *text, s32 x, s32 y, s32 color);

void func_800491F8(s32 value, s32 x, s32 y, s32 color)
{
    char buffer[0x20];

    func_8008CD30(buffer, D_8009F368, value);
    color &= 0xFFFF;
    func_80049100(buffer, x, y, color);
}
