#include "game/types.h"

struct Unk73700 {
    struct Unk73700 *next;
    void *first;
    void *second;
};

extern struct Unk73700 *D_8009F38C;

extern void func_80071FA4(struct Unk73700 *node, s32 arg);
extern void func_80072F7C(struct Unk73700 *node, s32 arg);

void func_80073700(s32 arg)
{
    struct Unk73700 *node;

    for (node = D_8009F38C; node != 0; node = node->next) {
        if (node->first != 0 || node->second != 0) {
            func_80071FA4(node, arg);
        } else {
            func_80072F7C(node, arg);
        }
    }
}
