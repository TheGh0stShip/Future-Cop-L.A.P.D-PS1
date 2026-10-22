#include "game/types.h"

struct PadPort {
    u8 pad_00[0x2C];
    u8 *payload;
    u8 pad_30[5];
    u8 payload_length;
    u8 command;
    u8 pad_37[0xF];
    u8 sequence_step;
    u8 pad_47[0xA];
    u8 requested_mode_index;
    u8 requested_mode_lock;
    u8 mode_already_selected;
    u8 pad_54[9];
    u8 actuator_map[6];
};

void func_8008B668(struct PadPort *port)
{
    s32 state = port->sequence_step;

    switch (state) {
    case 2:
        port->command = 0x44;
        port->payload = (u8 *)port + 0x51;
        port->payload_length = state;
        break;
    case 3:
        port->command = 0x4D;
        port->payload = port->actuator_map;
        port->payload_length = 6;
        break;
    }
}
