#include <array.h>
#include <limits.h>

TEST(get_push_get_pop_get_success)
{
    Array_i32* a = newArraya_i32(GLOBAL_ARENA, (i32[]){1, 2, 3, 4, 5}, 5);
    ASSERT(5 == get_i32(a, a->length - 1).result);
    ASSERT(5 == a->length);
    ASSERT(8 == a->capacity);
}

TEST(get_out_of_bounds_fail)
{
    Array_i32* a = newArraya_i32(GLOBAL_ARENA, (i32[]){1, 2, 3, 4, 5}, 5);

    Result_i32 i = get_i32(a, a->length);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);

    i = get_i32(a, INT_MAX);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);

    i = get_i32(a, INT_MIN);
    ASSERT(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERT(0 == i.result);
}

TEST(set_success)
{
    Array_i32* a = newArraya_i32(GLOBAL_ARENA, (i32[]){1, 2, 3}, 3);
    ASSERT(SUCCESS == set_i32(a, 1, 10));
    ASSERT(10 == get_i32(a, 1).result);
    ASSERT(ERROR_OUT_OF_BOUNDS == set_i32(a, INT_MAX, 1));
}
