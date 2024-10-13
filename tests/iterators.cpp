#include <fp/inplace_string.hpp>

#include <gtest/gtest.h>

TEST(Iterators, TestMutableForwardIteration)
{
    fp::inplace_string<10> str;
    std::size_t count = 0;
    for (auto it = str.begin(); it != str.end(); ++it) {
        count++;
    }
    ASSERT_EQ(count, str.size());
}

TEST(Iterators, TestConstForwardIteration)
{
    fp::inplace_string<10> str;
    std::size_t count = 0;
    for (auto it = str.cbegin(); it != str.cend(); ++it) {
        count++;
    }
    ASSERT_EQ(count, str.size());
}

TEST(Iterators, TestMutatableBackwardIteration)
{
    fp::inplace_string<10> str;
    std::size_t count = 0;
    for (auto it = str.rbegin(); it != str.rend(); ++it) {
        count++;
    }
    ASSERT_EQ(count, str.size());
}

TEST(Iterators, TestConstBackwardIteration)
{
    fp::inplace_string<10> str;
    std::size_t count = 0;
    for (auto it = str.crbegin(); it != str.crend(); ++it) {
        count++;
    }
    ASSERT_EQ(count, str.size());
}