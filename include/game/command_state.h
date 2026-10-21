#ifndef FUTURE_COP_COMMAND_STATE_H
#define FUTURE_COP_COMMAND_STATE_H

#include "types.h"

struct CommandState {
    u8 pad_00[0x20];
    void *source;
    u8 inline_data[8];
    void *value;
    u8 pad_30[4];
    u8 type;
    u8 active;
    u8 current;
    u8 previous;
};

#define OFFSET_OF(type, member) ((u32)&(((type *)0)->member))

typedef char assert_command_source_offset[
    OFFSET_OF(struct CommandState, source) == 0x20 ? 1 : -1];
typedef char assert_command_data_offset[
    OFFSET_OF(struct CommandState, inline_data) == 0x24 ? 1 : -1];
typedef char assert_command_value_offset[
    OFFSET_OF(struct CommandState, value) == 0x2C ? 1 : -1];
typedef char assert_command_type_offset[
    OFFSET_OF(struct CommandState, type) == 0x34 ? 1 : -1];
typedef char assert_command_active_offset[
    OFFSET_OF(struct CommandState, active) == 0x35 ? 1 : -1];
typedef char assert_command_current_offset[
    OFFSET_OF(struct CommandState, current) == 0x36 ? 1 : -1];
typedef char assert_command_previous_offset[
    OFFSET_OF(struct CommandState, previous) == 0x37 ? 1 : -1];

#undef OFFSET_OF

#endif
