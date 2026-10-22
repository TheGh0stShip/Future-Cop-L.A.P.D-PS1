#include "game/types.h"

extern void func_80091480(void);
extern void func_80096150(s32 option);

void func_80095E90(s32 option)
{
    if (option == 0) {
        func_80091480();
    }

    func_80096150(option);
}
