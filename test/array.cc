#include <gtest/gtest.h>

extern "C" {
#include "arena.h"
#include "array.h"
#include "types.h"
}

TEST(Array, COMPOSITE_pop_getr_success) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    EXPECT_EQ(1, a->capacity);

    push_i32(arena, a, 1);
    push_i32(arena, a, 2);
    push_i32(arena, a, 3);
    EXPECT_EQ(3, a->length);
    EXPECT_EQ(4, a->capacity);

    Result_i32 i = get_i32(a, a->length - 1);
    EXPECT_EQ(3, i.result);

    pop_i32(a);
    i = get_i32(a, a->length - 1);
    EXPECT_EQ(2, i.result);

    freeArena(arena);
}

TEST(Array, COMPOSITE_get_push_get_pop_get_success) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    EXPECT_EQ(1, a->capacity);

    push_i32(arena, a, 1);
    push_i32(arena, a, 2);
    push_i32(arena, a, 3);
    push_i32(arena, a, 4);
    push_i32(arena, a, 5);
    EXPECT_EQ(5, a->length);
    EXPECT_EQ(8, a->capacity);

    Result_i32 i = get_i32(a, a->length - 1);
    EXPECT_EQ(5, i.result);

    pop_i32(a);
    i = get_i32(a, a->length - 1);
    EXPECT_EQ(4, a->length);
    EXPECT_EQ(8, a->capacity);
    EXPECT_EQ(4, i.result);

    freeArena(arena);
}

TEST(Array, get_out_of_bounds_fail) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArrayc_i32(arena, 1);
    push_i32(arena, a, 1);
    push_i32(arena, a, 2);
    push_i32(arena, a, 3);
    push_i32(arena, a, 4);
    push_i32(arena, a, 5);
    EXPECT_EQ(5, a->length);
    EXPECT_EQ(8, a->capacity);

    Result_i32 i = get_i32(a, a->length);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = get_i32(a, INT_MAX);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = get_i32(a, INT_MIN);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    i = get_i32(a, INT_MIN + 1);
    EXPECT_EQ(true, i.fail);
    EXPECT_EQ(0, i.result);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);

    freeArena(arena);
}

// TEST(Array, set_success) {
//     Arena* arena = newArena(MEGABYTES(16));
//
//     Array_i32* a = newArrayc_i32(arena, 1);
//     push_i32(arena, a, 1);
//     push_i32(arena, a, 2);
//     push_i32(arena, a, 3);
//     EXPECT_EQ(3, a->length);
//     EXPECT_EQ(4, a->capacity);
//
//     freeArena(arena);
// }
