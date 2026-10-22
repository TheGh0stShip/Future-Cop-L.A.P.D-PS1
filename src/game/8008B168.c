#include "game/types.h"

struct PadPort {
    u8 pad_00[0x46];
    u8 sequence_step;
    u8 info_record_index;
    u8 combination_bytes_remaining;
};

extern void func_8008B750(struct PadPort *port, u8 index);
extern void func_8008B770(struct PadPort *port, u8 index);
extern void func_8008B790(struct PadPort *port, u8 index);
extern void func_8008B7B0(struct PadPort *port);

void func_8008B168(struct PadPort *port)
{
    s32 state = port->sequence_step;

    switch (state) {
    case 2:
        func_8008B750(port, port->info_record_index);
        break;
    case 3:
        func_8008B770(port, port->info_record_index);
        break;
    case 4:
        if (port->combination_bytes_remaining == 0) {
            func_8008B790(port, port->info_record_index);
        } else {
            func_8008B7B0(port);
        }
        break;
    }
}
