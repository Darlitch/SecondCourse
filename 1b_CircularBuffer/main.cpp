#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CircularBuffer.hpp"

int main(int argc, char** argv) {
    ::testing::InitGoogleMock(&argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Constructors, WithCapacity) {
    CircularBuffer cb(3);
    cb.push_back('a');
    cb.push_back('b');
    cb.push_back('c');
    ASSERT_EQ('a', cb.front());
    ASSERT_EQ('c', cb.back());
    cb.push_back('d');
    ASSERT_EQ('b', cb.front());
    ASSERT_EQ('d', cb.back());
    CircularBuffer cb2(1);
    cb2.push_back('a');
    cb2.push_back('b');
    ASSERT_EQ('a', cb2.front());
    ASSERT_EQ('b', cb2.back());
    cb2.push_back('c');
    ASSERT_EQ('b', cb2.front());
    ASSERT_EQ('c', cb2.back());
}

TEST(Constructors, Copy) {
    CircularBuffer cb(2);
    cb.push_back('a');
    cb.push_back('b');
    CircularBuffer cb2 = cb;
    ASSERT_EQ(cb.front(), cb2.front());
    ASSERT_EQ(cb.back(), cb2.back());
    ASSERT_EQ(cb.size(), cb2.size());
    ASSERT_EQ(cb.capacity(), cb2.capacity());
}

TEST(Constructors, WithCapacityAndElem) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ('c', cb.front());
    ASSERT_EQ('c', cb.back());
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb.capacity(), 2);
}

TEST(Modificators, PushBack) {
    CircularBuffer cb;
    cb.push_back('c');
    ASSERT_EQ('c', cb.back());
    cb.push_back('a');
    ASSERT_EQ('a', cb.back());
}

TEST(Modificators, PopBack) {
    CircularBuffer cb;
    cb.push_back('c');
    cb.pop_back();
    ASSERT_EQ(cb.empty(), true);
}