#include "game/types.h"

struct Unk71A68Entry {
    void *data;
    void *extra;
    s32 unk08;
    s32 unk0C;
};

struct Unk71A68 {
    s32 unk00;
    void *first;
    void *second;
    s16 count;
    s16 unk0E;
    struct Unk71A68Entry *entries;
};

extern void func_80070710(void *block);
extern void func_800181D4(void *block);

void func_80071A68(struct Unk71A68 *object)
{
    s32 i;

    if (object->first != 0) {
        func_80070710(object->first);
    }
    if (object->second != 0) {
        func_80070710(object->second);
    }
    for (i = 0; i < object->count; i++) {
        if (object->entries[i].extra != 0) {
            func_800181D4(object->entries[i].extra);
        }
        func_800181D4(object->entries[i].data);
    }
    func_800181D4(object->entries);
    func_800181D4(object);
}
