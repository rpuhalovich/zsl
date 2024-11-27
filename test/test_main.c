#include <arena.h>
#include <stdio.h>
#include <ztest.h>

#include "test_array.c"

int main(void)
{
    printf("\n");
    GLOBAL_ARENA = newArena(MEGABYTES(1));

    // array
    {
        get_push_get_pop_get_success();
        get_out_of_bounds_fail();
        set_success();
    }

    if (!hasError)
        pass();
    freeArena(GLOBAL_ARENA);

    printf("\n");
    return 0;
}
