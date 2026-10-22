#include "game/types.h"

struct ModelTable {
    s32 count;
};

struct ModelVertex {
    u8 unk00[4];
    u8 position[4];
};

struct ModelAnim {
    s32 count;
    struct ModelVertex *frames[1];
};

struct Model {
    u8 unk00[0x0C];
    u32 flags;
    u8 unk10[0x0C];
    struct ModelAnim *anim;
    u8 unk20[0x10];
    u8 parts[4];
    u8 unk34[4];
    struct ModelTable *table;
};

struct ModelInstance {
    struct Model *model;
    u8 coord[0x44];
    s16 frame;
};

extern void func_80010750(u8 *coord);
extern void func_80010234(u8 *vertices, s32 *out, s32 count);

void func_80017668(struct ModelInstance *instance, s32 part, s32 *out)
{
    struct Model *model;
    u8 *vertices;
    register struct ModelAnim *frame __asm__("$2");
    register u32 index __asm__("$16");

    model = instance->model;
    if ((model->flags >> 25) & 1) {
        vertices = (u8 *)(model->table + 1);
        vertices += instance->frame * model->table->count * 8;
        func_80010750(instance->coord);
        func_80010234(vertices + part * 8, out, 1);
    } else {
        index = model->parts[part];
        func_80010750(instance->coord);
        frame = (struct ModelAnim *)((u32)(instance->frame * 4) +
                                     (u32)model->anim);
        __asm__("" : : "r"(frame));
        index *= 8;
        __asm__("" : "=r"(index) : "0"(index));
        index += 4;
        func_80010234((u8 *)frame->frames[0] + index, out, 1);
    }
}
