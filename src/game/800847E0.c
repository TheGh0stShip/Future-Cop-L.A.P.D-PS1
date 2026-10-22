#include "game/types.h"

extern u8 *D_8009F9D8;
extern u8 *D_8009FABC;

#define COMPONENT(table, value, threshold) \
    (table)[(u8)(value) + ((((value) & 7) >= (threshold)) ? 8 : 0)]

#define PACK_PIXEL(table, red, green, blue, threshold) \
    ((COMPONENT((table), (red), (threshold)) << 10) | \
     (COMPONENT((table), (green), (threshold)) << 5) | \
     COMPONENT((table), (blue), (threshold)))

#define PACK_EVEN(words, output, pair, threshold) do { \
    u8 *_table = D_8009F9D8; \
    u32 _first = (words)[(pair) * 3 / 2]; \
    u32 _second = (words)[(pair) * 3 / 2 + 1]; \
    (output)[pair] = PACK_PIXEL(_table, _first >> 16, _first >> 8, _first, (threshold)[0]) | \
        (PACK_PIXEL(_table, _second >> 8, _second, _first >> 24, (threshold)[1]) << 16); \
} while (0)

#define PACK_ODD(words, output, pair, threshold) do { \
    u8 *_table = D_8009F9D8; \
    u32 _first = (words)[(pair) * 3 / 2]; \
    u32 _second = (words)[(pair) * 3 / 2 + 1]; \
    (output)[pair] = PACK_PIXEL(_table, _second, _first >> 24, _first >> 16, (threshold)[0]) | \
        (PACK_PIXEL(_table, _second >> 24, _second >> 16, _second >> 8, (threshold)[1]) << 16); \
} while (0)

void func_800847E0(u32 *source, u32 *destination, s16 *dimensions)
{
    u8 *threshold;
    u8 *thresholdEnd;
    s32 rows;

    threshold = D_8009FABC;
    rows = dimensions[3];
    thresholdEnd = threshold + 0x100;

    while (rows-- > 0) {
        PACK_EVEN(source, destination, 0, threshold + 0);
        PACK_ODD(source, destination, 1, threshold + 2);
        PACK_EVEN(source, destination, 2, threshold + 4);
        PACK_ODD(source, destination, 3, threshold + 6);
        PACK_EVEN(source, destination, 4, threshold + 8);
        PACK_ODD(source, destination, 5, threshold + 10);
        PACK_EVEN(source, destination, 6, threshold + 12);
        PACK_ODD(source, destination, 7, threshold + 14);

        source += 12;
        destination += 8;
        threshold += 16;
        if (threshold >= thresholdEnd) {
            threshold = D_8009FABC;
        }
    }
}
