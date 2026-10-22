#include "game/types.h"

struct CdlLOC {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
};

#define BCD_TO_INT(value) (((value) >> 4) * 10 + ((value) & 0xF))

s32 func_8008E720(struct CdlLOC *location)
{
    return (BCD_TO_INT(location->minute) * 60
            + BCD_TO_INT(location->second)) * 75
        + BCD_TO_INT(location->sector) - 150;
}
