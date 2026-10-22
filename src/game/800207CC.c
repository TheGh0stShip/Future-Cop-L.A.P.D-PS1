#include "game/types.h"

struct Packed6 {
    u8 bytes[6];
} __attribute__((packed));

struct Packed9 {
    u8 bytes[9];
} __attribute__((packed));

struct CardDirectoryFrame {
    u8 bytes[0x80];
};

struct CardManager {
    s32 unk00;
    s32 state[16];
    struct Packed9 *primary[16];
    struct Packed9 *secondary[16];
    u8 unk0C4[0x96C];
    struct CardDirectoryFrame directory[15];
};

extern struct CardManager *D_8009AB08;
extern const struct Packed9 D_80010AE8;
extern const struct Packed9 D_80010AF4;
extern const struct Packed9 D_80010B00;
extern const struct Packed6 D_8009F260;

extern void func_8001E318(void *destination, const void *source, s32 length);

void func_800207CC(void)
{
    struct CardDirectoryFrame *frame;
    s32 slot;

    D_8009AB08->state[0] = 1;
    *D_8009AB08->primary[0] = D_80010AE8;
    *D_8009AB08->secondary[0] = D_80010AE8;

    frame = D_8009AB08->directory;
    for (slot = 1; slot < 16; slot++, frame++) {
        u8 checksum;
        u8 *cursor;
        s32 i;

        checksum = 0;
        cursor = frame->bytes;
        for (i = 0; i < 0x7F; i++) {
            checksum ^= *cursor++;
        }
        if (*cursor != checksum) {
            frame->bytes[0] = 0x53;
        }

        switch (frame->bytes[0]) {
        case 0xA0:
        case 0xA1:
        case 0xA3:
            D_8009AB08->state[slot] = 2;
            *(struct Packed6 *)D_8009AB08->primary[slot] = D_8009F260;
            *(struct Packed6 *)D_8009AB08->secondary[slot] = D_8009F260;
            break;
        case 0x51:
            D_8009AB08->state[slot] = 1;
            func_8001E318(D_8009AB08->primary[slot], &frame->bytes[10], 20);
            *D_8009AB08->secondary[slot] = D_80010AF4;
            break;
        default:
            D_8009AB08->state[slot] = 1;
            *D_8009AB08->primary[slot] = D_80010B00;
            *D_8009AB08->secondary[slot] = D_80010B00;
            break;
        }
    }
}
