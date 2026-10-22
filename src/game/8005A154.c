#include "game/types.h"

struct Unk5A154 {
    u8 unk00[0xC1];
    s8 side;
};

struct Unk5A154Gauge {
    u8 unk00[0x13];
    u8 sound;
};

extern s32 D_8009FA58[];

extern s32 func_80046C84(s32 id, s32 arg1);

void func_8005A154(struct Unk5A154 *object, struct Unk5A154Gauge *gauge)
{
    func_80046C84(gauge->sound, D_8009FA58[object->side]);
}
