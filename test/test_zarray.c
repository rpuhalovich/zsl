#include <limits.h>
#include <zsl.h>

TEST(get_push_get_pop_get_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);
    ASSERTC(5 == get_i32(arr, arr->length - 1).result);
    ASSERTC(5 == arr->length);
    ASSERTC(8 == arr->capacity);

clean:
    freeArena(a);
}

TEST(get_out_of_bounds_fail)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);

    Result_i32 i = get_i32(arr, arr->length);
    ASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERTC(0 == i.result);

    i = get_i32(arr, INT_MAX);
    ASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERTC(0 == i.result);

    i = get_i32(arr, INT_MIN);
    ASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ASSERTC(0 == i.result);

clean:
    freeArena(a);
}

TEST(set_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3}, 3);
    ASSERTC(SUCCESS == set_i32(arr, 1, 10));
    ASSERTC(10 == get_i32(arr, 1).result);
    ASSERTC(ERROR_OUT_OF_BOUNDS == set_i32(arr, INT_MAX, 1));

clean:
    freeArena(a);
}
