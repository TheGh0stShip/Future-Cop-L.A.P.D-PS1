#include "game/types.h"

struct Resource {
    u8 unk00[0x0C];
    struct Resource *next;
    struct Resource *prev;
    u8 unk14[4];
    s32 group;
    s32 name;
};

extern struct Resource *D_8009F5B8;

s32 func_80019AA8(struct Resource *resource)
{
    struct Resource *node;

    for (node = D_8009F5B8; node != 0; node = node->next) {
        if (node->group == resource->group && node->name == resource->name) {
            break;
        }
    }
    return node != 0;
}
