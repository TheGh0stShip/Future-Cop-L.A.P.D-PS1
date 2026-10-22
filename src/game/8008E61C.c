#include "game/types.h"

struct CdlLOC {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
};

struct CdlLOC *func_8008E61C(s32 sector, struct CdlLOC *location)
{
    inline s32 encode_bcd(s32 value)
    {
        return ((value / 10) << 4) + (value % 10);
    }

    sector += 150;
    location->sector = encode_bcd(sector % 75);
    location->second = encode_bcd((sector / 75) % 60);
    location->minute = encode_bcd((sector / 75) / 60);
    return location;
}
