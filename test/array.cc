#include <gtest/gtest.h>

#include "types.h"
#include "arena.c"
#include "array.c"

TEST(Array, newArray) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArray_i32(arena);
    EXPECT_EQ(16, a->capacity);

    freeArena(arena);
}

TEST(Array, newArrayc) {
    Arena* arena = newArena(MEGABYTES(16));

    Array_i32* a = newArray_i32c(arena, 32);
    EXPECT_EQ(32, a->capacity);

    freeArena(arena);
}
