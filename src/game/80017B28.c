#include "game/types.h"

struct Unk17764 {
    u8 unk00[0x88];
    s32 id;
    struct Unk17764 *next;
};

extern struct Unk17764 *D_8009F588;

struct Unk17764 *func_80017B28(s32 id)
{
    struct Unk17764 *node;

    for (node = D_8009F588; node != 0; node = node->next) {
        if (id == node->id) {
            return node;
        }
    }
    return 0;
}
