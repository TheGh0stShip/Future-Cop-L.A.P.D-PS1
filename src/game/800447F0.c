#include "game/types.h"

struct Unk447F0 {
    u8 unk00[8];
    u16 period;
    s16 phase;
    u8 unk0C[0x18];
    void (*update)();
    u8 unk28[0x62];
    u16 basePeriod;
    u8 unk8C;
    u8 flags8D;
    u8 unk8E[0x1E];
};

extern void D_80044610();

extern void *func_80017EBC(s32 size, s32 tag);
extern u8 func_80043430(struct Unk447F0 *object, s32 def);
extern void func_800181D4(void *block);

struct Unk447F0 *func_800447F0(s32 def)
{
    struct Unk447F0 *object;

    object = func_80017EBC(0xAC, 1);
    if (object == 0) {
        return 0;
    }
    if (func_80043430(object, def) == 0) {
        func_800181D4(object);
        return 0;
    }
    object->update = D_80044610;
    object->period = object->basePeriod;
    if (object->flags8D & 0x30) {
        object->period = object->basePeriod << 1;
    }
    object->phase = 0;
    return object;
}
