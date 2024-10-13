#include <fp/inplace_string.hpp>

#include <gtest/gtest.h>

TEST(Iterators, TestForwardIteration)
{
    fp::inplace_string<10> str;
    std::size_t count = 0;
    for (const auto x : str) {
        count++;
    }
    ASSERT_EQ(count, str.size());
}