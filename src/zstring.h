#ifndef ZSTRING_H
#define ZSTRING_H

#include "zarena.h"
#include "zresult.h"
#include "ztypes.h"

FORWARD(sResult_char, Result_char)

// https://stackoverflow.com/questions/5290182/how-many-bytes-does-one-unicode-character-take
typedef struct sString {
    char* chars;
    u32 length;
} String;

String* newStringc(Arena* arena, char* str, u32 len);
String* newStrings(Arena* arena, String* str);
i32 cmp(String* str1, String* str2);
char* cstr(Arena* arena, String* str);
Result_char getChar(String* str, u32 i);

#endif // ZSTRING_H
