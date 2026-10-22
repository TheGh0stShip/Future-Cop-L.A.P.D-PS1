#include "game/types.h"

struct Unk60E28Table {
    s32 count;
    void *entries[0x80];
};

extern struct Unk60E28Table *D_8009F800;
extern void *func_80017EBC(s32 size, s32 tag);

void func_80060E28(void)
{
    struct Unk60E28Table *table;

    table = func_80017EBC(0x204, 2);
    D_8009F800 = table;
    table->count = 0;
}
