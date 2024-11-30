#ifndef ZMATH_H
#define ZMATH_H

#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vector2f;

typedef struct {
    i32 x;
    i32 y;
} Vector2i;

typedef struct {
    f32 x;
    f32 y;
    f32 width;
    f32 height;
} ZRectangle;

#endif // ZMATH_H
