#include "game/types.h"

struct Unk584BCAttacker {
    u8 unk00[8];
    s32 id;
};

struct Unk584BCHit {
    u8 unk00[4];
    s16 damage;
    u8 kind;
    u8 unk07;
    union {
        struct Unk584BCAttacker *attacker;
        s32 source;
    } from;
    u8 unk0C[0x0E];
    s16 amount;
    u8 unk1C[8];
    u32 flags;
};

struct Unk584BCInfo {
    u8 unk00[0x31A];
    s16 unk31A;
};

struct Unk584BC {
    u8 unk00[0x19];
    u8 flags19;
    u8 unk1A[0x92];
    struct Unk584BCInfo *info;
};

extern s32 func_800402DC(struct Unk584BC *owner, u8 column, s32 arg2, s32 row, s32 arg4);
extern void func_80058378(struct Unk584BC *object, s32 amount, s32 source);

s32 func_800584BC(s32 message, struct Unk584BC *object, struct Unk584BCHit *hit)
{
    s32 amount;
    s32 source;
    s32 kind;

    if (message == 0xE) {
        source = 0;
        if (hit->from.attacker != 0) {
            source = hit->from.attacker->id | 0x40000000;
        }
        amount = hit->damage;
        if (hit->kind == 0x10) {
            object->info->unk31A = 600;
            func_800402DC(object, 0xCD, 0, 1, 0);
        }
    } else {
        if (hit->flags & 0x40) {
            return 0;
        }
        amount = hit->amount;
        source = hit->from.source;
    }
    if (amount > 0) {
        kind = hit->kind;
        if (kind >= 2) {
            object->flags19 |= (kind << 3) & 0x7F0;
        }
        func_80058378(object, amount, source);
    }
    return 0;
}
