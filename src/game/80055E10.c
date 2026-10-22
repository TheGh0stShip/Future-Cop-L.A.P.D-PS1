#include "game/types.h"

struct InputAction {
    u32 buttons;
    s32 phase;
};

struct Unk55E10Control {
    u8 unk00[0x30];
    u32 buttons;
    struct InputAction fire;
    struct InputAction special;
    u8 unk44[8];
    struct InputAction weapon;
    u8 unk54[0x51];
    u8 weaponState;
    u8 unkA6[0x26A];
    s8 locked;
};

struct Unk55E10 {
    u8 unk00[0xAC];
    struct Unk55E10Control *control;
};

extern void func_8005780C(u32 buttons, struct InputAction *action);
extern void func_8006C34C(struct Unk55E10 *object, s32 arg1);
extern void func_8006BA50(struct Unk55E10 *object, s32 arg1);

void func_80055E10(struct Unk55E10 *object)
{
    struct Unk55E10Control *control;
    u32 buttons;

    control = object->control;
    if (control->locked != 0) {
        return;
    }
    buttons = control->buttons;
    func_8005780C(buttons, &control->special);
    func_8005780C(control->buttons, &control->fire);
    func_8005780C(control->buttons, &control->weapon);
    if (control->fire.phase == 0) {
        func_8006C34C(object, 0);
    }
    if (buttons & 0x4000000) {
        func_8006BA50(object, 0);
    }
    if (buttons & 0x400000) {
        func_8006BA50(object, 1);
    }
    if (control->weapon.phase == control->weaponState) {
        func_8006BA50(object, 2);
    }
}
