#include "game/types.h"

struct Unk471C4Info {
    u8 unk00[0xC1];
    s8 stream;
};

struct Unk471C4Object {
    u8 unk00[0xAC];
    struct Unk471C4Info *info;
};

struct Unk471C4Stream {
    s32 playing;
    s32 track;
    s32 handle;
};

extern struct Unk471C4Stream D_800A1810[];

extern void func_800243F8(s32 handle);
extern void func_80046E88(s32 track, s32 arg1, s32 arg2, s32 arg3, s32 handle,
                          struct Unk471C4Object *object);

void func_800471C4(s32 track, struct Unk471C4Object *object)
{
    struct Unk471C4Stream *stream;

    stream = &D_800A1810[object->info->stream];
    if (track == stream->track) {
        return;
    }
    if (stream->playing != 0) {
        func_800243F8(stream->handle);
        stream->playing = 0;
    }
    stream->track = track;
    if (track == 0xFF) {
        return;
    }
    func_80046E88(track, 0, 0, -1, stream->handle, object);
    stream->playing = 1;
}
