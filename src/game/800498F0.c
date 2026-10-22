#include "game/types.h"

struct GridObject {
    s16 unk00;
    s16 cell;
    u8 unk04[0x3C];
    struct GridObject *next;
};

extern struct GridObject **D_8009F780;
extern s32 D_8009F784;

extern void func_800195D0(s32 pool, struct GridObject *object);

void func_800498F0(struct GridObject *object)
{
    struct GridObject **cell;
    struct GridObject *node;

    if (object->cell >= 0) {
        cell = &D_8009F780[object->cell];
        if (*cell == object) {
            *cell = object->next;
        } else {
            node = *cell;
            while (node != 0) {
                if (node->next == object) {
                    node->next = object->next;
                    break;
                }
                node = node->next;
            }
        }
    }
    func_800195D0(D_8009F784, object);
}
