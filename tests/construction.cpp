#include <fp/inplace_string.hpp>

#include <gtest/gtest.h>

TEST(Construction, DefaultConstruction)
{
    fp::inplace_string<10> str;
    ASSERT_EQ(str.size(), 0);
    ASSERT_TRUE(str.empty());
    ASSERT_EQ(str.capacity(), 10);
}

TEST(Construction, RepeatCharConstructor)
{
    fp::inplace_string<10> str(5, 'a');
    ASSERT_EQ(str.size(), 5);
    ASSERT_EQ(std::string_view{str}, std::string_view{"aaaaa"});
}