#include <gtest/gtest.h>

#include "types.h"
#include "arena.c"
#include "array.c"

TEST(Array, aPop) {
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

TEST(Array, aGet_aAppend_aGet_aPop_aGet) {
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
