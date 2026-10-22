#include "game/types.h"

struct PadPort {
    u8 pad_00[0x46];
    u8 sequence_step;
    u8 info_record_index;
    u8 pad_48[0x9C];
    u8 current_mode_index;
};

extern void func_8008B73C(struct PadPort *port);
extern void func_8008B750(struct PadPort *port, u8 index);
extern void func_8008B790(struct PadPort *port, u8 index);

void func_8008AE8C(struct PadPort *port)
{
    s32 state = port->sequence_step;

    switch (state) {
    case 2:
        func_8008B73C(port);
        break;
    case 3:
        func_8008B750(port, port->current_mode_index);
        break;
    case 4:
        func_8008B790(port, port->info_record_index);
        break;
    }
}
