#include <arena.h>
#include <stdio.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

Arena* GLOBAL_ARENA = NULL;

void assert_impl(bool condition, i32 line, char* file)
{
    if (!condition) {
        printf(ANSI_COLOR_RED"\nASSERTION FAILED: %s:%d\n\n"ANSI_COLOR_RESET, file, line);
        freeArena(GLOBAL_ARENA);
        exit(1);
    }
}

#define ASSERT(condition) assert_impl(condition, __LINE__, __FILE__)

#include "test_array.c"

int main(void)
{
    GLOBAL_ARENA = newArena(KILOBYTES(1));

    // array
    {
        get_push_get_pop_get_success();
        get_out_of_bounds_fail();
        set_success();
    }

    printf(ANSI_COLOR_GREEN"\nPASS\n\n"ANSI_COLOR_RESET);
    freeArena(GLOBAL_ARENA);
    return 0;
}
