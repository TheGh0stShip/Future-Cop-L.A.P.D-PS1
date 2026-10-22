#include "../../include/game/types.h"

struct DisplayRecord {
    u8 pad_00[2];
    s16 right;
    u8 pad_04[4];
    s16 left;
};

struct DisplayOwner {
    u8 pad_00[0x2DC];
    struct DisplayRecord record;
};

struct DisplayState {
    u8 pad_00[0xAC];
    struct DisplayOwner *owner;
};

void func_80051AC8(struct DisplayState *state, int compact)
{
    struct DisplayRecord *record = &state->owner->record;
    s32 center = 0x126;

    if (compact != 0) {
        center = 0xCC;
    }
    record->right = center + 0x19;
    record->left = center - 0x19;
}
