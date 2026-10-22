#include "game/types.h"

struct Unk77884 {
    u8 unk00;
    u8 unk01;
    u8 unk02;
    s8 unk03;
    u8 unk04;
    u8 unk05;
    s16 unk06;
    u8 unk08;
    u8 unk09;
    u8 unk0A;
    u8 unk0B;
    s32 unk0C;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
};

void func_80077884(struct Unk77884 *state, s32 arg1, u8 arg2)
{
    state->unk03 = -1;
    state->unk06 = 100;
    state->unk0A = 0xA0;
    state->unk02 = arg2;
    state->unk00 = 0;
    state->unk01 = 0;
    state->unk0C = 0;
    state->unk10 = 0;
    state->unk14 = 0;
    state->unk1C = 0;
    state->unk05 = 0;
    state->unk04 = 0;
    state->unk24 = 0;
    state->unk18 = 0;
    state->unk20 = 0;
    state->unk28 = arg1;
    state->unk08 = 0;
    state->unk09 = 0;
    state->unk0B = 0x6E;
}
