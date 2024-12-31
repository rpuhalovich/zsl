TEST(min_i32)
{
    ZASSERT(0 == min_i32(1, 0));
    ZASSERT(-1 == min_i32(-1, 0));
    ZASSERT(INT_MIN == min_i32(INT_MIN, INT_MAX));
}

TEST(max_i32)
{
    ZASSERT(1 == max_i32(1, 0));
    ZASSERT(0 == max_i32(-1, 0));
    ZASSERT(INT_MAX == max_i32(INT_MIN, INT_MAX));
}

TEST(clamp_i32)
{
    ZASSERT(1 == clamp_i32(1, 0, 100));
    ZASSERT(10 == clamp_i32(1, 10, 100));
    ZASSERT(100 == clamp_i32(101, 10, 100));
    ZASSERT(-1 == clamp_i32(-10, -1, 0));
}
