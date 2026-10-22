#include "game/types.h"

extern void func_8008C9C0(void);
extern void func_8008D6E0(s32 mode);
extern void func_8008D660(s32 channel);
extern void func_8008D74C(void);
extern void func_8008C950(void);
extern void func_8008C9E0(void);

void func_800200A8(void)
{
    func_8008C9C0();
    func_8008D6E0(0);
    func_8008D660(0);
    func_8008D74C();
    func_8008C950();
    func_8008C9E0();
}
