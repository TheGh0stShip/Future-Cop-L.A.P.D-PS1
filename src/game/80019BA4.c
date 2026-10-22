#include "game/types.h"

struct Resource {
    u8 unk00[0x0C];
    struct Resource *next;
};

extern struct Resource *D_8009F5B8;
extern struct Resource *D_8009F5BC;

extern void func_80019AF8(struct Resource *resource);

void func_80019BA4(void)
{
    struct Resource *node;
    struct Resource *current;

    node = D_8009F5B8;
    while (node != 0) {
        current = node;
        node = node->next;
        func_80019AF8(current);
    }
    D_8009F5B8 = 0;
    D_8009F5BC = 0;
}
