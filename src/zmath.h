#ifndef ZMATH_H
#define ZMATH_H

#include "ztypes.h"

typedef struct {
    f32 x;
    f32 y;
} v2f;

typedef struct {
    i32 x;
    i32 y;
} v2i;

typedef struct {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
} rec;

#endif // ZMATH_H
