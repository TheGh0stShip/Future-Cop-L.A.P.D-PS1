#include "game/types.h"

struct Unk75478 {
    u8 unk00[0x24];
    void *light;
};

extern void func_800498F0(void *object);

void func_80075478(struct Unk75478 *object)
{
    if (object->light != 0) {
        func_800498F0(object->light);
        object->light = 0;
    }
}
