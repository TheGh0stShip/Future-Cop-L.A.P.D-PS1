#include "game/types.h"

struct Camera {
    u8 unk00[0x18];
    s32 roll;
    s32 pitch;
    s32 yaw;
    u8 unk24[4];
    u8 matrix[0x30];
    u8 rotation[0x20];
};

extern struct Camera *D_8009F9E4;

extern void func_800188D0(s32 yaw, s32 pitch, s32 roll, u8 *rotation);
extern void func_80018860(u8 *rotation, u8 *matrix);

void func_80017860(s32 yaw, s32 pitch, s32 roll)
{
    func_800188D0(yaw, pitch, roll, D_8009F9E4->rotation);
    func_80018860(D_8009F9E4->rotation, D_8009F9E4->matrix);
    D_8009F9E4->roll = roll;
    D_8009F9E4->pitch = pitch;
    D_8009F9E4->yaw = yaw;
}
