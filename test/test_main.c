#include <ztest.h>

#include "test_array.c"
#include "test_zstring.c"

int main(void)
{
    TEST_BEGIN(MEGABYTES(1));

    // array
    {
        TEST(get_push_get_pop_get_success);
        TEST(get_out_of_bounds_fail);
        TEST(set_success);
    }

    // string
    {
        TEST(String_cmp);
        TEST(String_cstr_strncmp);
        TEST(String_getChar);
    }

    TEST_END();
    return 0;
}
