#include "game/types.h"

struct PadPort {
    u8 pad_00[0x14];
    void (*command_handler)(void *);
    s32 (*response_handler)(struct PadPort *);
    u8 pad_1C[4];
    u8 *requested_actuator_map;
    u8 pad_24[0x22];
    u8 sequence_step;
};

extern s32 (*D_8009C744)(struct PadPort *port);
extern void func_8008B4EC(struct PadPort *port);
extern s32 func_8008B508(struct PadPort *port);

s32 func_8008B484(struct PadPort *port, u8 *command)
{
    s32 result;
    s32 active;

    result = D_8009C744(port);
    if (result != 0) {
        return 0;
    } else {
        result = 1;
        __asm__ volatile("" : "=r"(result) : "0"(result));
        active = 1;
        port->sequence_step = active;
        port->command_handler = (void (*)(void *))func_8008B4EC;
        port->requested_actuator_map = command;
        port->response_handler = func_8008B508;
    }
    return result;
}
