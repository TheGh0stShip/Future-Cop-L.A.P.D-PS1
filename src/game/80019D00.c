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

extern void func_800181D4(void *block);

void func_80019D00(struct Resource *resource)
{
    struct Resource *node;

    for (node = D_8009F5B8; node != 0; node = node->next) {
        if (node->group == resource->group && node->name == resource->name) {
            break;
        }
    }
    func_800181D4(resource);
}
