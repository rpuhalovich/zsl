#include <zstring.h>

void newStringc_strncmp(void)
{
    char* teststr = "hello";
    String* s = newStringc(GLOBAL_ARENA, teststr);
    ASSERT(s->length == strlen(teststr));
    ASSERT(0 == strncmp(teststr, s->chars, s->length));
}

void newStrings_cmp(void)
{
    char* teststr = "hello";
    String* s1 = newStringc(GLOBAL_ARENA, teststr);
    String* s2 = newStrings(GLOBAL_ARENA, s1);

    ASSERT(s1->length == s2->length);
    ASSERT(0 == cmp(s1, s2));
    ASSERT(s1->chars != s2->chars);
}

void newStrings_cstr_strncmp(void)
{
    char* teststr = "hello";
    String* s1 = newStringc(GLOBAL_ARENA, teststr);
    ASSERT(0 == strncmp(teststr, cstr(GLOBAL_ARENA, s1), strlen(teststr)));
}
