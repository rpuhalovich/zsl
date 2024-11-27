#ifndef ZASSERT_H
#define ZASSERT_H

#include <arena.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

Arena* GLOBAL_ARENA = NULL;

void pass(void)
{
    printf(ANSI_COLOR_GREEN"\nPASS\n\n"ANSI_COLOR_RESET);
}

void assert_impl(bool condition, i32 line, char* file)
{
    if (!condition) {
        printf(ANSI_COLOR_RED"\nASSERTION FAILED: %s:%d\n\n"ANSI_COLOR_RESET, file, line);
        freeArena(GLOBAL_ARENA);
        exit(1);
    }
}

#define ASSERT(condition) assert_impl(condition, __LINE__, __FILE__)

#endif // ZASSERT_H
