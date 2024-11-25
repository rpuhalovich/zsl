#include <string.h>

#include "zstring.h"

String* newString(Arena* arena)
{
    String* s = allocate(arena, sizeof(String));
    s->characters = newArray_char(arena);
    return s;
}

String* newStringc(Arena* arena, char* str)
{
    String* s = allocate(arena, sizeof(String));
    s->characters = newArraya_char(arena, str, strlen(str));
    return s;
}

String* newStrings(Arena* arena, String* str)
{
    String* s = allocate(arena, sizeof(String));
    s->characters = newArraya_char(arena, str->characters->items, str->characters->length);
    return s;
}

i32 cmp(String* str1, String* str2)
{
    char s1len = str1->characters->length;
    char s2len = str2->characters->length;
    i32 res =
        strncmp(str1->characters->items, str2->characters->items, s1len < s2len ? s1len : s2len);
    return res;
}
