#include <gtest/gtest.h>

#include "types.h"
#include "arena.c"
#include "array.c"

TEST(Array, COMPOSITE_aPop_aGet_success) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    EXPECT_EQ(1, a->capacity);

    aAppend_i32(arena, a, 1);
    aAppend_i32(arena, a, 2);
    aAppend_i32(arena, a, 3);
    EXPECT_EQ(3, a->length);
    EXPECT_EQ(4, a->capacity);

    i32 i = aGet_i32(a, a->length - 1);
    EXPECT_EQ(3, i);

    aPop_i32(a);
    i = aGet_i32(a, a->length - 1);
    EXPECT_EQ(2, i);

    freeArena(arena);
}

TEST(Array, COMPOSITE_aGet_aAppend_aGet_aPop_aGet_success) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    EXPECT_EQ(1, a->capacity);

    aAppend_i32(arena, a, 1);
    aAppend_i32(arena, a, 2);
    aAppend_i32(arena, a, 3);
    aAppend_i32(arena, a, 4);
    aAppend_i32(arena, a, 5);
    EXPECT_EQ(5, a->length);
    EXPECT_EQ(8, a->capacity);

    i32 i = aGet_i32(a, a->length - 1);
    EXPECT_EQ(5, i);

    aPop_i32(a);
    i = aGet_i32(a, a->length - 1);
    EXPECT_EQ(4, a->length);
    EXPECT_EQ(8, a->capacity);
    EXPECT_EQ(4, i);

    freeArena(arena);
}

TEST(Array, aGet_out_of_bounds_fail) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    aAppend_i32(arena, a, 1);
    aAppend_i32(arena, a, 2);
    aAppend_i32(arena, a, 3);
    aAppend_i32(arena, a, 4);
    aAppend_i32(arena, a, 5);
    EXPECT_EQ(5, a->length);
    EXPECT_EQ(8, a->capacity);

    Result_i32 i = aGetr_i32(a, a->length);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = aGetr_i32(a, INT_MAX);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = aGetr_i32(a, INT_MIN);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = aGetr_i32(a, INT_MIN + 1);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    freeArena(arena);
}
