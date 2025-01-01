#include <limits.h>

TEST(get_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);
    ZASSERTC(5 == get_i32(arr, arr->length - 1).result);
    ZASSERTC(5 == arr->length);
    ZASSERTC(8 == arr->capacity);

clean:
    freeArena(a);
}

TEST(get_out_of_bounds_fail)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3, 4, 5}, 5);

    Result_i32 i = get_i32(arr, arr->length);
    ZASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ZASSERTC(0 == i.result);

    i = get_i32(arr, INT_MAX);
    ZASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ZASSERTC(0 == i.result);

    i = get_i32(arr, INT_MIN);
    ZASSERTC(ERROR_OUT_OF_BOUNDS == i.error);
    ZASSERTC(0 == i.result);

clean:
    freeArena(a);
}

TEST(set_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_i32* arr = newArraya_i32(a, (i32[]){1, 2, 3}, 3);
    ZASSERTC(SUCCESS == set_i32(arr, 1, 10));
    ZASSERTC(10 == get_i32(arr, 1).result);
    ZASSERTC(ERROR_OUT_OF_BOUNDS == set_i32(arr, INT_MAX, 1));

clean:
    freeArena(a);
}

TEST(push_many_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_u32* arr = newArray_u32(a);
    for (u32 i = 0; i < 128; i++)
        push_u32(a, arr, i);

    ZASSERTC(arr->length == 128);
    ZASSERTC(arr->capacity == 256);

clean:
    freeArena(a);
}

TEST(insert_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_u32* arr = newArrayc_u32(a, 8);
    push_u32(a, arr, 1);
    push_u32(a, arr, 2);
    push_u32(a, arr, 3);

    insert_u32(a, arr, 1, 10);

    ZASSERTC(1 == get_u32(arr, 0).result);
    ZASSERTC(10 == get_u32(arr, 1).result);
    ZASSERTC(2 == get_u32(arr, 2).result);
    ZASSERTC(3 == get_u32(arr, 3).result);

    ZASSERTC(4 == arr->length);
    ZASSERTC(8 == arr->capacity);

clean:
    freeArena(a);
}

TEST(insert_newcapacity_success)
{
    Arena* a = newArena(KILOBYTES(16));

    Array_u32* arr = newArrayc_u32(a, 4);
    push_u32(a, arr, 1);
    push_u32(a, arr, 2);
    push_u32(a, arr, 3);

    insert_u32(a, arr, 1, 10);

    ZASSERTC(1 == get_u32(arr, 0).result);
    ZASSERTC(10 == get_u32(arr, 1).result);
    ZASSERTC(2 == get_u32(arr, 2).result);
    ZASSERTC(3 == get_u32(arr, 3).result);

    ZASSERTC(4 == arr->length);
    ZASSERTC(8 == arr->capacity);

clean:
    freeArena(a);
}
