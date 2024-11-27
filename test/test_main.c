#include <arena.h>
#include <stdio.h>
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
        newStringc_strncmp();
        newStrings_cmp();
        newStrings_cstr_strncmp();
    }

    TEST_END();
    return 0;
}
