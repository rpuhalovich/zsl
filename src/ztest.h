#ifndef ZTEST_H
#define ZTEST_H

#include <arena.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

int errorCount = 0;
int assert_impl(bool condition, i32 line, char* file)
{
    if (!condition) {
        printf(ANSI_COLOR_RED "  ASSERTION FAILED %s:%d\n" ANSI_COLOR_RESET, file, line);
        return 1;
    }
    return 0;
}

#define ASSERT(condition)                                                                          \
    if (assert_impl(condition, __LINE__, __FILE__)) {                                              \
        errorCount++;                                                                              \
        return;                                                                                    \
    }

typedef struct {
    void (*tests[10000])(void);
    char* testNames[10000];
    u32 numTests;
} ZTestContext;
ZTestContext zTestContext;

// Taken from: <https://github.com/jasmcaus/tau>
// Execute scope underneath before main function exection
#if defined(_MSC_VER)
    #if defined(_WIN64)
        #define ZSL_SYMBOL_PREFIX
    #else
        #define ZSL_SYMBOL_PREFIX "_"
    #endif // _WIN64

    #pragma section(".CRT$XCU", read)
    #define ZSL_TEST_INITIALIZER(f)                                                                \
        static void __cdecl f(void);                                                               \
        __pragma(comment(linker, "/include:" ZSL_SYMBOL_PREFIX #f "_")) ZSL_C_FUNC                 \
            __declspec(allocate(".CRT$XCU")) void(__cdecl * f##_)(void) = f;                       \
        static void __cdecl f(void)
#else
    #define ZSL_TEST_INITIALIZER(f)                                                                \
        static void f(void) __attribute__((constructor));                                          \
        static void f(void)
#endif // _MSC_VER

#define TEST(testName)                                                                             \
    static void _TEST_##testName(void);                                                            \
    ZSL_TEST_INITIALIZER(_REGISTER_TEST_##testName)                                                \
    {                                                                                              \
        i32 index = zTestContext.numTests++;                                                       \
        zTestContext.tests[index] = &_TEST_##testName;                                             \
        const char* name = #testName;                                                              \
        zTestContext.testNames[index] = malloc(strlen(name) + 1);                                  \
        strncpy(zTestContext.testNames[index], name, strlen(name) + 1);                            \
    }                                                                                              \
    void _TEST_##testName(void)

Arena* GLOBAL_ARENA = NULL;
static i32 zTestMain(u64 globalArenaNumBytes);
inline i32 zTestMain(u64 globalArenaNumBytes)
{
    GLOBAL_ARENA = newArena(globalArenaNumBytes);

    for (u32 i = 0; i < zTestContext.numTests; i++) {
        printf(ANSI_COLOR_GREEN "RUN %s...\n" ANSI_COLOR_RESET, zTestContext.testNames[i]);
        zTestContext.tests[i]();
        clear(GLOBAL_ARENA);
    }
    printf("\n");

    if (errorCount == 0)
        printf(ANSI_COLOR_GREEN "PASS\n" ANSI_COLOR_RESET);
    else
        printf(ANSI_COLOR_RED "FAIL\n" ANSI_COLOR_RESET);

    printf("Test count: %d\n", zTestContext.numTests);
    if (errorCount > 0)
        printf("Fail count: %d\n", errorCount);

    for (u32 i = 0; i < zTestContext.numTests; i++)
        free(zTestContext.testNames[i]);

    freeArena(GLOBAL_ARENA);
    return 0;
}

#define ZTEST_MAIN(globalArenaNumBytes)                                                            \
    ZTestContext zTestContext = {0};                                                               \
    int main(void)                                                                                 \
    {                                                                                              \
        return zTestMain(globalArenaNumBytes);                                                     \
    }

#endif // ZTEST_H
