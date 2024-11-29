#include <ztest.h>

#include "test_array.c"
#include "test_zstring.c"

int main(void)
{
    TEST_BEGIN(MEGABYTES(1));

    // array
    {
        get_push_get_pop_get_success();
        get_out_of_bounds_fail();
        set_success();
    }

    // string
    {
        String_cmp();
        String_cstr_strncmp();
        String_getChar();
    }

    TEST_END();
    return 0;
}
