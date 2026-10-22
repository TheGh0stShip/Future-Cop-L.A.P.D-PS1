#include "game/types.h"

extern s32 func_8008FD30(void *destination, s32 size);

s32 func_8008E5B8(void *destination, s32 size)
{
    return func_8008FD30(destination, size) == 0;
}
