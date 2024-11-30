#ifndef ZTEST_H
#define ZTEST_H

#include <arena.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

Arena* GLOBAL_ARENA = NULL;
int hasError = 0;

void pass(void)
{
    printf(ANSI_COLOR_GREEN "PASS\n" ANSI_COLOR_RESET);
}

int assert_impl(bool condition, i32 line, char* file)
{
    if (!condition) {
        printf(ANSI_COLOR_RED "     ASSERTION FAILED: %s:%d\n" ANSI_COLOR_RESET, file, line);
        return 1;
    }
    return 0;
}

#define ASSERT(condition)                                                                          \
    if (assert_impl(condition, __LINE__, __FILE__)) {                                              \
        hasError = 1;                                                                              \
        return;                                                                                    \
    }

#define TEST(testName)                                                                             \
    printf(ANSI_COLOR_GREEN "RUNNING TEST: %s...\n" ANSI_COLOR_RESET, #testName);                  \
    testName();

#define TEST_BEGIN(capacity)                                                                       \
    printf("\n");                                                                                  \
    GLOBAL_ARENA = newArena(capacity);

#define TEST_END()                                                                                 \
    freeArena(GLOBAL_ARENA);                                                                       \
    if (!hasError)                                                                                 \
        printf(ANSI_COLOR_GREEN "PASS\n" ANSI_COLOR_RESET);                                        \
    else                                                                                           \
        printf(ANSI_COLOR_RED "FAIL\n" ANSI_COLOR_RESET);                                          \
    printf("\n");

#endif // ZTEST_H
