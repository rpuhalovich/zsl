#ifndef ZSTRING_H
#define ZSTRING_H

#include "arena.h"
#include "array.h"
#include "types.h"

// https://stackoverflow.com/questions/5290182/how-many-bytes-does-one-unicode-character-take
typedef struct {
    char* chars;
    u32 length;
} String;

String* newString(Arena* arena);

String* newStringc(Arena* arena, char* str);

String* newStrings(Arena* arena, String* str);

i32 cmp(String* str1, String* str2);

char* cstr(Arena* arena, String* str);

Result_char getChar(String* str, u32 i);

#endif // ZSTRING_H
