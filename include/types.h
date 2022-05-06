#pragma once

typedef unsigned long long addr_t;
typedef addr_t ptr_t;

typedef unsigned char bit;

typedef unsigned char u8;
typedef char s8;

typedef unsigned short u16;
typedef short s16;

typedef unsigned u32;
typedef int s32;

typedef unsigned long long u64;
typedef long long s64;

typedef float f32;
typedef double f64;

typedef float d32;
typedef double d64;


typedef u8 byte_t;
typedef u16 halfword;
typedef u32 word;
typedef u64 dword;

//#define LOG
#define LOG_LEN 34


#define CONV_S32(val)   (*((s32*)&val))
#define CONV_U32(val)   (*((u32*)&val))
