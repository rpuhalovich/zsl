TEST(String_cmp)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s1 = newStringc(a, teststr, strlen(teststr));
    String* s2 = newStrings(a, s1);

    ZASSERTC(s1->length == s2->length);
    ZASSERTC(0 == cmp(s1, s2));
    ZASSERTC(s1->chars != s2->chars);

clean:
    freeArena(a);
}

TEST(String_cstr_strncmp)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s = newStringc(a, teststr, strlen(teststr));
    ZASSERTC(0 == strncmp(teststr, cstr(a, s), strlen(teststr)));

clean:
    freeArena(a);
}

TEST(String_getChar)
{
    Arena* a = newArena(KILOBYTES(16));

    char* teststr = "hello";
    String* s = newStringc(a, teststr, strlen(teststr));
    ZASSERTC('h' == getChar(s, 0).result);
    ZASSERTC(SUCCESS == getChar(s, 0).error);

    ZASSERTC('o' == getChar(s, s->length - 1).result);
    ZASSERTC(SUCCESS == getChar(s, s->length - 1).error);

    ZASSERTC(0 == getChar(s, s->length).result);
    ZASSERTC(ERROR_OUT_OF_BOUNDS == getChar(s, s->length).error);

clean:
    freeArena(a);
}
