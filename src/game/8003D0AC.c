#include "game/types.h"

struct Unk3CF20 {
    struct Unk3CF20 *next;
};

extern struct Unk3CF20 *D_8009F320;
extern void func_8003CF20(struct Unk3CF20 *object);

void func_8003D0AC(void)
{
    struct Unk3CF20 *object;
    struct Unk3CF20 *next;

    object = D_8009F320;
    while (object != 0) {
        next = object->next;
        func_8003CF20(object);
        object = next;
    }
    D_8009F320 = 0;
}
