#include "game/types.h"

struct Unk3FA34Link {
    struct Unk3FA34Link *next;
    void *object;
};

extern void *D_8009F70C;
extern struct Unk3FA34Link *D_8009F708;
extern s32 D_8009FA00[];

extern void func_8003BC0C(void *object);
extern u8 func_8003B838(void *object);
extern struct Unk3FA34Link *func_800195A4(void *pool);

void func_8003FA34(void *object)
{
    struct Unk3FA34Link *link;

    if (D_8009F70C == 0 || D_8009FA00[0] == 4 || D_8009FA00[0] == 0xD) {
        func_8003BC0C(object);
        return;
    }
    if (func_8003B838(object) != 0) {
        link = func_800195A4(D_8009F70C);
        if (link != 0) {
            link->object = object;
            if (D_8009F708 == 0) {
                link->next = 0;
            } else {
                link->next = D_8009F708;
            }
            D_8009F708 = link;
        }
    }
}
