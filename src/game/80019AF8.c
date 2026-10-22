#include "game/types.h"

struct Resource {
    u8 unk00[8];
    void (*release)(struct Resource *resource);
    struct Resource *next;
    struct Resource *prev;
};

extern struct Resource *D_8009F5B8;
extern struct Resource *D_8009F5BC;

extern void func_800181D4(void *block);

void func_80019AF8(struct Resource *resource)
{
    if (resource->release != 0) {
        resource->release(resource);
    }
    if (resource->prev != 0) {
        resource->prev->next = resource->next;
    } else {
        D_8009F5B8 = resource->next;
        if (D_8009F5B8 != 0) {
            D_8009F5B8->prev = 0;
        }
    }
    if (resource->next != 0) {
        resource->next->prev = resource->prev;
    } else {
        D_8009F5BC = resource->prev;
        if (D_8009F5BC != 0) {
            D_8009F5BC->next = 0;
        }
    }
    func_800181D4(resource);
}
