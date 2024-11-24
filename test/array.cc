#include <gtest/gtest.h>

extern "C" {
#include "arena.h"
#include "array.h"
#include "types.h"
}

TEST(Array, pop_get_success) {
    Arena* arena = newArena(KILOBYTES(1));

    Array_i32* a = newArraya_i32(arena, (i32[]){1, 2, 3}, 3);
    EXPECT_EQ(3, a->length);
    EXPECT_EQ(4, a->capacity);

    EXPECT_EQ(3, get_i32(a, a->length - 1).result);
    pop_i32(a);
    EXPECT_EQ(2, get_i32(a, a->length - 1).result);

    freeArena(arena);
}

TEST(Array, get_push_get_pop_get_success) {
    Arena* arena = newArena(KILOBYTES(1));

    Array_i32* a = newArraya_i32(arena, (i32[]){1, 2, 3, 4, 5}, 5);
    EXPECT_EQ(5, get_i32(a, a->length - 1).result);

    pop_i32(a);
    pop_i32(a);
    EXPECT_EQ(3, a->length);
    EXPECT_EQ(8, a->capacity);
    EXPECT_EQ(3, get_i32(a, a->length - 1).result);

    freeArena(arena);
}

TEST(Array, get_out_of_bounds_fail) {
    Arena* arena = newArena(KILOBYTES(1));

    Array_i32* a = newArraya_i32(arena, (i32[]){1, 2, 3, 4, 5}, 5);

    Result_i32 i = get_i32(a, a->length);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);
    EXPECT_EQ(0, i.result);

    i = get_i32(a, INT_MAX);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);
    EXPECT_EQ(0, i.result);

    i = get_i32(a, INT_MIN);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);
    EXPECT_EQ(0, i.result);

    i = get_i32(a, INT_MIN + 1);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, i.error);
    EXPECT_EQ(0, i.result);

    freeArena(arena);
}

TEST(Array, set_success) {
    Arena* arena = newArena(KILOBYTES(1));

    Array_i32* a = newArraya_i32(arena, (i32[]){1, 2, 3}, 3);
    set_i32(a, 1, 10);
    EXPECT_EQ(10, get_i32(a, 1).result);

    set_i32(a, 1, 10);
    EXPECT_EQ(ERROR_OUT_OF_BOUNDS, set_i32(a, INT_MAX, 1));

    freeArena(arena);
}
