#include "game/types.h"

extern u16 D_800A1E10[][0x10];
extern s32 D_8009F360;

void func_800488A4(s32 index, s32 blue, s32 green, s32 red)
{
    register u16 (*table)[0x10] __asm__("$2");
    register u16 *row __asm__("$3");
    register u16 *color __asm__("$14");
    register s32 i __asm__("$12");
    register s32 b __asm__("$9");
    register s32 g __asm__("$10");
    register s32 r __asm__("$11");
    register s32 mask __asm__("$13");
    register s32 highBit __asm__("$15");
    register s32 greenShift __asm__("$24");
    register s32 blueShift __asm__("$25");

    if (index < 0 || D_8009F360 < index) {
        return;
    }
    blueShift = 10;
    greenShift = 5;
    table = D_800A1E10;
    __asm__ volatile ("" : "+r" (table));
    row = table[index];
    __asm__ volatile ("" : "+r" (row));
    row[0] = 0;
    i = 2;
    highBit = 0x8000;
    color = &row[2];
    red &= 0xFF;
    r = red * 2;
    green &= 0xFF;
    g = green * 2;
    blue &= 0xFF;
    b = blue * 2;
    for (; i < 0x11; i++) {
        register s32 value __asm__("$2");
        register s32 bluePart __asm__("$3");
        register s32 greenValue __asm__("$8");
        register s32 redValue __asm__("$4");

        mask = 0;
        if (i < 8) {
            mask = highBit;
        }
        value = b;
        if (value < 0) {
            value += 15;
        }
        bluePart = (value >> 4) << blueShift;
        greenValue = g;
        if (greenValue < 0) {
            greenValue += 15;
        }
        value = greenValue >> 4;
        greenValue = value << greenShift;
        redValue = r;
        if (redValue < 0) {
            redValue += 15;
        }
        value = redValue >> 4;
        bluePart |= greenValue;
        value |= bluePart;
        value = mask | value;
        color[-1] = value;
        color++;
        r += red;
        g += green;
        b += blue;
    }
}
