#include "game/types.h"

struct Unk6CA68 {
    void *first;
    void *second;
};

extern void func_800181D4(void *block);

void func_8006CA68(struct Unk6CA68 *object)
{
    func_800181D4(object->first);
    func_800181D4(object->second);
    func_800181D4(object);
}
