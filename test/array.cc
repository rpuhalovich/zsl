#include <gtest/gtest.h>

#include "types.h"
#include "arena.c"
#include "array.c"

Arena* arena = newArena(MEGABYTES(16));

TEST(Array, newArray) {
    Array_i32* a = newArray_i32(arena);
    EXPECT_EQ(16, a->capacity);
}
