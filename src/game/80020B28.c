#include "../../include/game/types.h"

struct TableRecord {
    u8 bytes[4];
    u8 pad_04[4];
    u16 value;
    u8 pad_0A[0x76];
};

void func_80020B28(struct TableRecord *records)
{
    u32 index;

    for (index = 0x10; index < 0x24; index++) {
        records[index].bytes[0] = 0xFF;
        records[index].bytes[1] = 0xFF;
        records[index].bytes[2] = 0xFF;
        records[index].bytes[3] = 0xFF;
        records[index].value = 0xFFFF;
    }
}
