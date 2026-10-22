#include "game/types.h"

struct PadPort;

struct PadCapabilityRecord {
    u8 protocol[2];
    u8 payload_bytes;
    u8 activation_cost;
    u8 high_bit;
};

struct PadCombinationRecord {
    u8 length;
    u8 reserved[3];
    u8 *data;
};

struct PadPort {
    u16 *mode_table;
    struct PadCapabilityRecord *capabilities;
    struct PadCombinationRecord *combinations;
    u8 pad_0C[8];
    void (*command_handler)(void *);
    s32 (*response_handler)(struct PadPort *);
    u8 pad_1C[0x2A];
    u8 sequence_step;
    u8 info_record_index;
    u8 pad_48;
    u8 communication_state;
    u8 pad_4A[0x99];
    u8 mode_count;
    u8 pad_E4[5];
    u8 actuator_count;
};

extern s32 (*D_8009C744)(void);
extern void func_8008B168(struct PadPort *port);
extern s32 func_8008B210(struct PadPort *port);

s32 func_8008B094(struct PadPort *port, u8 *buffer)
{
    register s32 cursor __asm__("$5");
    s32 result;
    s32 state;
    register s32 (*process_function)(struct PadPort *);
    register s32 row_count __asm__("$3");
    s32 column_count;

    if (buffer == 0) {
        goto return_zero;
    }
    if (port->capabilities != 0) {
        return 0;
    }
    if (D_8009C744() != 0) {
return_zero:
        return 0;
    }

    result = 1;
    __asm__ volatile("" : "=r"(result) : "0"(result));
    state = 4;
    cursor = ((s32)buffer + 3) >> 2;
    port->communication_state = state;
    state = 1;
    port->sequence_step = state;
    port->command_handler = (void (*)(void *))func_8008B168;
    __asm__ volatile("" ::: "memory");
    row_count = *(volatile u8 *)&port->mode_count;
    __asm__ volatile("" : "=r"(row_count) : "0"(row_count));
    process_function = func_8008B210;
    port->response_handler = process_function;
    __asm__ volatile("" ::: "memory");
    column_count = port->actuator_count;
    __asm__ volatile("" : "=r"(row_count), "=r"(column_count), "=r"(result)
                     : "0"(row_count), "1"(column_count), "2"(result));

    cursor <<= 2;
    port->mode_table = (u16 *)cursor;
    port->info_record_index = 0;
    __asm__ volatile("" ::: "memory");
    cursor += ((row_count + 1) >> 1) * 4;
    port->capabilities = (struct PadCapabilityRecord *)cursor;
    cursor += (column_count * 5 + 3) & 0xFFC;
    port->combinations = (struct PadCombinationRecord *)cursor;
    return result;
}
