#ifndef ZSTRING_H
#define ZSTRING_H

#include "zarena.h"
#include "zarray.h"
#include "ztypes.h"

// https://stackoverflow.com/questions/5290182/how-many-bytes-does-one-unicode-character-take
typedef struct {
    char* chars;
    u32 length;
} String;

String* newString(Arena* arena);
String* newStringc(Arena* arena, char* str, u32 len);
String* newStrings(Arena* arena, String* str);
i32 cmp(String* str1, String* str2);
char* cstr(Arena* arena, String* str);
Result_char getChar(String* str, u32 i);

#ifdef ZSL_STRING_IMPLEMENTATION
    #include <string.h>

String* newStringc(Arena* arena, char* str, u32 len)
{
    String* s = allocate(arena, sizeof(String));
    s->length = len;
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

Result_char getChar(String* str, u32 i)
{
    if (i < 0 || i >= str->length)
        return fail_char(ERROR_OUT_OF_BOUNDS);
    return success_char(str->chars[i]);
}
#endif // ZSL_STRING_IMPLEMENTATION

#endif // ZSTRING_H
