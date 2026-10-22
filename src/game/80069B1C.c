#include "game/types.h"

struct Unk69B1C {
    u8 unk00[0xE4];
    void *unkE4;
    void *unkE8;
    void *unkEC;
    void *unkF0;
};

extern struct Unk69B1C *D_8009FA94;
extern u8 D_8009FAA4[];
extern u8 D_8009FAB4[];
extern u8 D_8009FA70[];
extern u8 D_8009FACC[];

void func_80069B1C(void)
{
    struct Unk69B1C *object;

    object = D_8009FA94;
    object->unkE4 = D_8009FAA4;
    object->unkE8 = D_8009FAB4;
    object->unkEC = D_8009FA70;
    object->unkF0 = D_8009FACC;
}
