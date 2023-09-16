#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "Stack.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(TestStack, TestPushAndPop) {
    Stack<int> stk;
    int value = 12;
    stk.Push(value);
    int value2 = stk.Pop();
    ASSERT_EQ(value, value2);
}