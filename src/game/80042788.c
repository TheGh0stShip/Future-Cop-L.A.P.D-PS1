#include "game/types.h"

struct Unk42764Node {
    struct Unk42764Node *next;
};

struct Unk42764Owner {
    u8 unk00[0x64];
    struct Unk42764Node *free;
    struct Unk42764Node *active;
    u8 unk6C[0x38];
    u16 count;
};

void func_80042788(struct Unk42764Owner *owner, struct Unk42764Node *node)
{
    node->next = owner->active;
    owner->active = node;
    owner->count++;
}
