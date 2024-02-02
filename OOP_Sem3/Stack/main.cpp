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
    value = 26;
    stk.Push(value);
    value2 = stk.Pop();
    ASSERT_EQ(value, value2);
}

TEST(TestStack, TestSize) {
    Stack<int> stk;
    ASSERT_EQ(stk.Size(), 0);
    int value = 12;
    stk.Push(value);
    ASSERT_EQ(stk.Size(), 1);
    value = 25;
    stk.Push(value);
    ASSERT_EQ(stk.Size(), 2);
    stk.Pop();
    ASSERT_EQ(stk.Size(), 1);
}

TEST(TestStack, TestTop) {
    Stack<int> stk;
    int value = 12;
    stk.Push(value);
    ASSERT_EQ(stk.Top(), value);
    value = 26;
    stk.Push(value);
    ASSERT_EQ(stk.Top(), value);
}

TEST(TestStack, TestEmpty) {
    Stack<int> stk;
    ASSERT_EQ(stk.Empty(), true);
    int value = 12;
    stk.Push(value);
    ASSERT_EQ(stk.Empty(), false);
}

TEST(TestStack, TestResize) {
    Stack<int> stk;
    int value = 12, value2 = 24, value3 = 36;
    stk.Push(value);
    stk.Push(value2);
    stk.Push(value3);
    ASSERT_EQ(stk.Pop(), 36);
    ASSERT_EQ(stk.Pop(), 24);
    ASSERT_EQ(stk.Pop(), 12);
}
