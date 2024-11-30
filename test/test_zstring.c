#include <zstring.h>

void String_cmp(void)
{
    char* teststr = "hello";
    String* s1 = newStringc(GLOBAL_ARENA, teststr, strlen(teststr));
    String* s2 = newStrings(GLOBAL_ARENA, s1);

    ASSERT(s1->length == s2->length);
    ASSERT(0 == cmp(s1, s2));
    ASSERT(s1->chars != s2->chars);
}

void String_cstr_strncmp(void)
{
    char* teststr = "hello";
    String* s = newStringc(GLOBAL_ARENA, teststr, strlen(teststr));
    ASSERT(0 == strncmp(teststr, cstr(GLOBAL_ARENA, s), strlen(teststr)));
}

void String_getChar(void)
{
    char* teststr = "hello";
    String* s = newStringc(GLOBAL_ARENA, teststr, strlen(teststr));
    ASSERT('h' == getChar(s, 0).result);
    ASSERT(SUCCESS == getChar(s, 0).error);

    ASSERT('o' == getChar(s, s->length - 1).result);
    ASSERT(SUCCESS == getChar(s, s->length - 1).error);

    ASSERT(0 == getChar(s, s->length).result);
    ASSERT(ERROR_OUT_OF_BOUNDS == getChar(s, s->length).error);
}
