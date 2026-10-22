#include "game/types.h"

struct CdlLOC {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
};

#define INT_TO_BCD(value) ((((value) / 10) << 4) + ((value) % 10))

struct CdlLOC *func_8008E61C(s32 sector, struct CdlLOC *location)
{
    s32 time;

    sector += 150;
    time = sector / 75;
    location->sector = INT_TO_BCD(sector % 75);
    location->second = INT_TO_BCD(time % 60);
    location->minute = INT_TO_BCD(time / 60);
    return location;
}
