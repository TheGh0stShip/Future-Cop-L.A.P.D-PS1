#include "game/types.h"

struct PadMapping {
    u32 masks[14];
};

extern struct PadMapping D_800AAC50[];

void func_8001E588(u32 port)
{
    struct PadMapping *mapping;

    if (port < 2) {
        mapping = &D_800AAC50[port];
        mapping->masks[0] = 0x100000;
        mapping->masks[1] = 0x400000;
        mapping->masks[2] = 0x800000;
        mapping->masks[3] = 0x200000;
        mapping->masks[4] = 0x80000000;
        mapping->masks[5] = 0x40000000;
        mapping->masks[6] = 0x20000000;
        mapping->masks[7] = 0x10000000;
        mapping->masks[8] = 0x8000000;
        mapping->masks[9] = 0x4000000;
        mapping->masks[10] = 0x1000000;
        mapping->masks[11] = 0x2000000;
        mapping->masks[12] = 0x10000;
        mapping->masks[13] = 0x80000;
    }
}
