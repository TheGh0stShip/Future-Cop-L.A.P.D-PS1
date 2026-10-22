#include "game/types.h"

struct Unk5E074 {
    void *model;
};

extern void func_8006CA68(void *model);
extern void func_800181D4(void *block);

void func_8005E074(struct Unk5E074 *effect)
{
    func_8006CA68(effect->model);
    func_800181D4(effect);
}
