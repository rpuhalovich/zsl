#ifndef TYPES_H
#define TYPES_H

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define f32 float
#define f64 double

#if defined(_MSC_VER) || defined(__SYMBIAN32__)
typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned long u64;
typedef signed long i64;
#else
    #include <stdint.h>
typedef uint8_t u8;
typedef int8_t i8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
#endif

#ifdef ZDEBUG
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif

#define STRUCT_FORWARD(type)                                                                       \
    struct _##type;                                                                                \
    typedef struct _##type type;

#endif // TYPES_H
