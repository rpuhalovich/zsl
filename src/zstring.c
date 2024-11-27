#include <string.h>

#include "zstring.h"

String* newStringc(Arena* arena, char* str)
{
    String* s = allocate(arena, sizeof(String));
    s->length = strlen(str);
    s->chars = allocate(arena, sizeof(char) * s->length);
    memcpy(s->chars, str, s->length);
    return s;
}

String* newStrings(Arena* arena, String* str)
{
    String* s = allocate(arena, sizeof(String));
    s->chars = allocate(arena, sizeof(char) * str->length);
    s->length = str->length;
    memcpy(s->chars, str->chars, str->length);
    return s;
}

i32 cmp(String* str1, String* str2)
{
    u32 s1len = str1->length;
    u32 s2len = str2->length;
    i32 res = strncmp(str1->chars, str2->chars, s1len < s2len ? s1len : s2len);
    return res;
}

char* cstr(Arena* arena, String* str)
{
    char* res = allocate(arena, sizeof(char) * str->length + 1);
    memcpy(res, str->chars, str->length);
    res[str->length] = '\0';
    return res;
}
