#include "game/types.h"

struct PadPort {
    u8 pad_00[0x46];
    u8 sequence_step;
    u8 pad_47[0xC];
    u8 mode_already_selected;
};

extern void (*D_8009C72C)(struct PadPort *port);

s32 func_8008B6BC(struct PadPort *port)
{
    if (port->mode_already_selected != 0) {
        if (port->sequence_step == 2) {
            return 1;
        }
        port->sequence_step = 0xFE;
        return 0;
    }

    D_8009C72C(port);
    return 0;
}
