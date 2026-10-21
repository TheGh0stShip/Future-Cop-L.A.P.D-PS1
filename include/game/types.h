#ifndef FUTURE_COP_GAME_TYPES_H
#define FUTURE_COP_GAME_TYPES_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef char assert_s8_size[sizeof(s8) == 1 ? 1 : -1];
typedef char assert_u16_size[sizeof(u16) == 2 ? 1 : -1];
typedef char assert_u32_size[sizeof(u32) == 4 ? 1 : -1];

#endif
