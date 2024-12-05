#include <limits.h>
#include <zarray.h>

TEST(get_push_get_pop_get_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);
    ASSERT(5 == get_i32(arr, arr->length - 1).result);
    ASSERT(5 == arr->length);
    ASSERT(8 == arr->capacity);

    freeArena(a);
}

TEST(get_out_of_bounds_fail)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);

    Result_i32 i = get_i32(arr, arr->length);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);

    i = get_i32(arr, INT_MAX);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);

    i = get_i32(arr, INT_MIN);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);

    freeArena(a);
}

TEST(set_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3}, 3);
    ASSERT(SUCCESS == set_i32(arr, 1, 10));
    ASSERT(10 == get_i32(arr, 1).result);
    ASSERT(ERROR_OUT_OF_BOUNDS == set_i32(arr, INT_MAX, 1));

    freeArena(a);
}
