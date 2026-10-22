#include "game/types.h"

struct Unk17764 {
    u8 unk00[0x8C];
    struct Unk17764 *next;
};

extern struct Unk17764 *D_8009F588;

extern void func_800181D4(void *block);

void func_80017764(void)
{
    struct Unk17764 *node;
    struct Unk17764 *current;

    node = D_8009F588;
    while (node != 0) {
        current = node;
        node = node->next;
        func_800181D4(current);
    }
    D_8009F588 = 0;
}
