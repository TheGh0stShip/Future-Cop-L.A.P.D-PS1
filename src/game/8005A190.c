#include "game/types.h"

struct Unk5A190Gauge {
    s32 unk00;
    s32 value;
    s32 unk08;
    s32 limit;
    u8 state;
    u8 unk11[3];
};

struct Unk5A190 {
    u8 unk00[0x54];
    struct Unk5A190Gauge gauges[1];
};

extern void func_8005A154(struct Unk5A190 *object, struct Unk5A190Gauge *gauge, s32 rising);

void func_8005A190(struct Unk5A190 *object, s32 index, s32 value)
{
    struct Unk5A190Gauge *gauge;

    if (value < 0) {
        value = 0;
    }
    gauge = &object->gauges[index];
    gauge->value = value;
    if (value >= gauge->limit) {
        gauge->state = 2;
        return;
    }
    if (gauge->state == 2) {
        gauge->state = 1;
        func_8005A154(object, gauge, 1);
    } else if (gauge->state == 1 && value <= 0) {
        gauge->state = 0;
        func_8005A154(object, gauge, 0);
    }
}
