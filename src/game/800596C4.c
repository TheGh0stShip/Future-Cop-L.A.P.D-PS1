#include "game/types.h"

struct Unk596C4Part {
    u8 unk00[0x1C];
    s32 x;
    s32 y;
    s32 z;
    u8 unk28[0x24];
};

struct Unk596C4State {
    u8 unk00[0x178];
    struct Unk596C4Part parts[2];
    u8 unk210[0xEC];
    s8 part;
};

struct Unk596C4 {
    u8 unk00[0xAC];
    struct Unk596C4State *state;
};

struct Unk596C4Effect {
    u8 unk00[0x10];
    s32 color;
};

extern s32 D_8009AF88;
extern s32 D_8009AF94[];

extern void func_80017668(struct Unk596C4Part *part, s32 arg1, s32 *out);
extern u32 func_80019668(void);
extern void func_80014C64(s32 *from, s32 *to, struct Unk596C4Effect *effect);

void func_800596C4(struct Unk596C4 *object)
{
    struct Unk596C4Part *part;
    s32 from[3];
    s32 to[3];
    s32 index;
    s32 z;

    index = object->state->part;
    if (index < 0) {
        return;
    }
    part = &object->state->parts[index];
    func_80017668(part, 0, from);
    to[0] = from[0] + ((part->x << 4) >> 8);
    to[1] = from[1] + ((part->y << 4) >> 8);
    z = from[2] + ((part->z << 4) >> 8);
    __asm__("" : : "r"(z));
    to[2] = z;
    D_8009AF88 = D_8009AF94[func_80019668() % 3];
    func_80014C64(from, to,
                  (struct Unk596C4Effect *)((u8 *)&D_8009AF88 - 0x10));
}
