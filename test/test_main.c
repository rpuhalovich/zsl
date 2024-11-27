#include <arena.h>
#include <stdio.h>
#include "zassert.h"

#include "test_array.c"

int main(void)
{
    GLOBAL_ARENA = newArena(MEGABYTES(1));

    // array
    {
        get_push_get_pop_get_success();
        get_out_of_bounds_fail();
        set_success();
    }

    pass();
    freeArena(GLOBAL_ARENA);
    return 0;
}
