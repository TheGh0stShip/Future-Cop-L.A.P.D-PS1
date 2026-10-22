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

struct Resource *func_80019BF0(s32 group, s32 name)
{
    struct Resource *node;

    for (node = D_8009F5B8; node != 0; node = node->next) {
        if (node->group == group && node->name == name) {
            return node;
        }
    }
    return 0;
}
