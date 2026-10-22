#include "game/types.h"

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
    u8 pad_0C[0x30];
    u8 *response;
    u8 pad_40[6];
    u8 sequence_step;
    u8 info_record_index;
    u8 combination_bytes_remaining;
    u8 communication_state;
    u8 pad_4A[0x99];
    u8 mode_count;
    u8 pad_E4[5];
    u8 actuator_count;
    u8 combination_count;
};

extern u8 *D_800A3B80;

s32 func_8008B210(struct PadPort *input_port)
{
    register struct PadPort *port = input_port;

    switch (port->sequence_step) {
    case 2:
        port->mode_table[port->info_record_index] =
            port->response[5] + (port->response[4] << 8);
        port->info_record_index++;
        if (port->info_record_index >= port->mode_count) {
            port->info_record_index = 0;
            break;
        }
        return 0;

    case 3: {
        struct PadCapabilityRecord *record =
            port->capabilities + port->info_record_index;
        record->protocol[0] = port->response[4];
        record->protocol[1] = port->response[5] & 0x7F;
        record->payload_bytes = port->response[6];
        record->activation_cost = port->response[7];
        {
            register s32 high = port->response[5];
            record->high_bit = high >> 7;
        }
        port->info_record_index++;
        if (port->info_record_index >= port->actuator_count) {
            port->info_record_index = 0;
            port->combination_bytes_remaining = 0;
            break;
        }
        return 0;
    }

    case 4: {
        struct PadCombinationRecord *record =
            port->combinations + port->info_record_index;
        register u8 *source;
        register u8 *base;
        register s32 bytes __asm__("$4");
        u32 offset;

        if (port->combination_bytes_remaining == 0) {
            register s32 length = port->response[4];
            bytes = 3;
            port->combination_bytes_remaining = length;
            record->length = port->combination_bytes_remaining;
            {
                register u8 *response = port->response;
                register s32 index = port->info_record_index;
                source = response + 5;
                if (index == 0) {
                    base = (u8 *)port->combinations;
                    offset = port->combination_count * 8;
                } else {
                    base = record[-1].data;
                    offset = (record[-1].length + 3) & 0x1FC;
                }
            }
            base += offset;
            record->data = base;
            D_800A3B80 = base;
        } else {
            register u8 *response = port->response;
            bytes = 6;
            source = response + 2;
        }

        bytes--;
        if (bytes != -1) {
            register u8 **destination = &D_800A3B80;
            do {
                register s32 remaining = port->combination_bytes_remaining;
                bytes--;
                if (!remaining) {
                    goto exhausted;
                }
                {
                    register u8 *output = *destination;
                    register s32 value = *source++;
                    __asm__("" : "=r"(source) : "0"(source));
                    *output = value;
                    *destination = output + 1;
                }
                port->combination_bytes_remaining--;
            } while (bytes != -1);
        }

        if (port->combination_bytes_remaining != 0) {
zero:
            return 0;
        }
exhausted:
        port->info_record_index++;
        if (port->info_record_index >= port->combination_count) {
            port->communication_state = 6;
            port->sequence_step = 0xFE;
            return 0;
        }
        port->combination_bytes_remaining = 0;
        goto zero;
    }

    default:
        return 1;
    }

    return 1;
}
