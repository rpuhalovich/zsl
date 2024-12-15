#include <zsl.h>

TEST(String_cmp)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s1 = newStringc(a, teststr, strlen(teststr));
    String* s2 = newStrings(a, s1);

    ASSERTC(s1->length == s2->length);
    ASSERTC(0 == cmp(s1, s2));
    ASSERTC(s1->chars != s2->chars);

clean:
    freeArena(a);
}

TEST(String_cstr_strncmp)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s = newStringc(a, teststr, strlen(teststr));
    ASSERTC(0 == strncmp(teststr, cstr(a, s), strlen(teststr)));

clean:
    freeArena(a);
}

TEST(String_getChar)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s = newStringc(a, teststr, strlen(teststr));
    ASSERTC('h' == getChar(s, 0).result);
    ASSERTC(SUCCESS == getChar(s, 0).error);

    ASSERTC('o' == getChar(s, s->length - 1).result);
    ASSERTC(SUCCESS == getChar(s, s->length - 1).error);

    ASSERTC(0 == getChar(s, s->length).result);
    ASSERTC(ERROR_OUT_OF_BOUNDS == getChar(s, s->length).error);

clean:
    freeArena(a);
}
