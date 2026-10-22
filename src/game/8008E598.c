#include "game/types.h"

extern s32 func_8008FC30(void *destination, s32 size);

s32 func_8008E598(void *destination, s32 size)
{
    return func_8008FC30(destination, size) == 0;
}
