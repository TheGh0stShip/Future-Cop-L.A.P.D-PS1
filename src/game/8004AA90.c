#include "game/types.h"

struct Unk4AA90 {
    u8 unk00[0x48];
    struct Unk4AA90 *next;
};

extern struct Unk4AA90 *D_8009F798;

struct Unk4AA90 *func_8004AA90(void)
{
    struct Unk4AA90 *current;

    current = D_8009F798;
    if (current->next == 0) {
        goto empty;
    }
    D_8009F798 = current->next;
    goto done;
empty:
    current = 0;
done:
    return current;
}
