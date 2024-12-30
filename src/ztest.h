#ifndef ZTEST_H
#define ZTEST_H

#include <stdio.h>
#include <stdlib.h>

#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_RESET "\x1b[0m"

int errorCount = 0;
#define ZASSERT(condition)                                                                         \
  if (!(condition)) {                                                                              \
    printf(ANSI_COLOR_RED "  ASSERTION FAILED %s:%d\n" ANSI_COLOR_RESET, __FILE__, __LINE__);      \
    errorCount++;                                                                                  \
    return;                                                                                        \
  }

#define ZASSERTC(condition)                                                                        \
  if (!(condition)) {                                                                              \
    printf(ANSI_COLOR_RED "  ASSERTION FAILED %s:%d\n" ANSI_COLOR_RESET, __FILE__, __LINE__);      \
    errorCount++;                                                                                  \
    goto clean;                                                                                    \
  }

typedef struct {
  void (*tests[10000])(void);
  char* testNames[10000];
  int numTests;
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
  #define ZSL_TEST_INITIALIZER(f)                                                                  \
    static void __cdecl f(void);                                                                   \
    __pragma(comment(linker, "/include:" ZSL_SYMBOL_PREFIX #f "_")) ZSL_C_FUNC                     \
        __declspec(allocate(".CRT$XCU")) void(__cdecl * f##_)(void) = f;                           \
    static void __cdecl f(void)
#else
  #define ZSL_TEST_INITIALIZER(f)                                                                  \
    static void f(void) __attribute__((constructor));                                              \
    static void f(void)
#endif // _MSC_VER

#define TEST(testName)                                                                             \
  static void _TEST_##testName(void);                                                              \
  ZSL_TEST_INITIALIZER(_REGISTER_TEST_##testName)                                                  \
  {                                                                                                \
    int index = zTestContext.numTests++;                                                           \
    zTestContext.tests[index] = &_TEST_##testName;                                                 \
    const char* name = #testName;                                                                  \
    zTestContext.testNames[index] = malloc(strlen(name) + 1);                                      \
    strncpy(zTestContext.testNames[index], name, strlen(name) + 1);                                \
  }                                                                                                \
  void _TEST_##testName(void)

static int zTestMain(void);
inline int zTestMain(void)
{
  for (int i = 0; i < zTestContext.numTests; i++) {
    printf(ANSI_COLOR_GREEN "RUN %s...\n" ANSI_COLOR_RESET, zTestContext.testNames[i]);
    zTestContext.tests[i]();
  }
  printf("\n");

  if (errorCount == 0)
    printf(ANSI_COLOR_GREEN "PASS\n" ANSI_COLOR_RESET);
  else
    printf(ANSI_COLOR_RED "FAIL\n" ANSI_COLOR_RESET);

  printf("Test count: %d\n", zTestContext.numTests);
  if (errorCount > 0)
    printf("Fail count: %d\n", errorCount);

  for (int i = 0; i < zTestContext.numTests; i++)
    free(zTestContext.testNames[i]);

  return 0;
}

#define ZTEST_MAIN()                                                                               \
  ZTestContext zTestContext = {0};                                                                 \
  int main(void)                                                                                   \
  {                                                                                                \
    return zTestMain();                                                                            \
  }

#endif // ZTEST_H
