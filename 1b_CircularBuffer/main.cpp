#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "CircularBuffer.hpp"

using CircularBuffer = Buffer::CircularBuffer;

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

TEST(Operators, Staples) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ('c', cb[1]);
    cb[1] = 'b';
    ASSERT_EQ('b', cb[1]);
    ASSERT_EQ('b', cb.back());
}

// TEST(Operators, Assignment) {

// }

// TEST(Operators, Equal) {

// }

// TEST(Operators, NotEqual) {

// }

TEST(Selectors, IsLinearized) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.is_linearized(), true);
    cb.push_back('c');
    ASSERT_EQ(cb.is_linearized(), false);
}

TEST(Selectors, Full) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.full(), true);
    cb.pop_back();
    ASSERT_EQ(cb.full(), false);
    cb.push_back('a');
    ASSERT_EQ(cb.full(), true);
    cb.push_back('b');
    ASSERT_EQ(cb.full(), true);
}

TEST(Selectors, Reserve) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.reserve(), 0);
    cb.pop_back();
    ASSERT_EQ(cb.reserve(), 1);
    cb.push_back('a');
    ASSERT_EQ(cb.reserve(), 0);
    cb.push_back('b');
    ASSERT_EQ(cb.reserve(), 0);
}

TEST(Selectors, Capacity) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.capacity(), 2);
    CircularBuffer cb2;
    ASSERT_EQ(cb2.capacity(), 50);
    CircularBuffer cb3(10);
    ASSERT_EQ(cb3.capacity(), 10);
}

TEST(Modificators, Linearize) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.is_linearized(), true);
    cb.push_back('c');
    ASSERT_EQ(cb.is_linearized(), false);
    cb.linearize();
    ASSERT_EQ(cb.is_linearized(), true);
    CircularBuffer cb2(4, 'c');
    cb2.push_back('b');
    ASSERT_EQ('b', cb2[3]);
    ASSERT_EQ(cb2.is_linearized(), false);
    cb2.linearize();
    ASSERT_EQ('c', cb2[0]);
    ASSERT_EQ(cb2.is_linearized(), true);
}

TEST(Modificators, Rotate) {
    CircularBuffer cb(4, 'c');
    cb.push_back('a');
    cb.push_back('b');
    ASSERT_EQ(cb.at(2), 'a');
    ASSERT_EQ(cb.at(3), 'b');
    cb.rotate(2);
    ASSERT_EQ(cb[0], 'a');
    cb.rotate(2);
    ASSERT_EQ(cb.at(2), 'a');
}

TEST(Modificators, SetCapacity) {
    CircularBuffer cb(4, 'c');
    ASSERT_EQ(cb.capacity(), 4);
    ASSERT_EQ(cb.size(), 4);
    cb.set_capacity(2);
    ASSERT_EQ(cb.capacity(), 2);
    ASSERT_EQ(cb.size(), 2);
    cb.push_back('a');
    cb.push_back('b');
    cb.set_capacity(4);
    ASSERT_EQ(cb.capacity(), 4);
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb.at(0), 'a');
    ASSERT_EQ(cb.at(1), 'b');
    ASSERT_EQ(cb.size(), 2);
}

TEST(Modificators, Resize) {
    CircularBuffer cb(2, 'c');
    cb.resize(4, 'a');
    ASSERT_EQ(cb.size(), 4);
    ASSERT_EQ(cb.capacity(), 4);
    ASSERT_EQ(cb.at(0), 'c');
    ASSERT_EQ(cb.at(1), 'c');
    ASSERT_EQ(cb.at(2), 'a');
    ASSERT_EQ(cb.at(3), 'a');
    cb.resize(2);
    ASSERT_EQ(cb.size(), 2);
    ASSERT_EQ(cb.front(), 'c');
    ASSERT_EQ(cb.back(), 'c');
}

// TEST(Modificators, Swap) {

// }

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

// TEST(Modificators, PushFront) {

// }

// TEST(Modificators, PopFront) {

// }

// TEST(Modificators, Insert) {

// }

// TEST(Modificators, Erase) {

// }

TEST(Modificators, Clear) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.empty(), false);
    ASSERT_EQ(cb.size(), 2);
    cb.clear();
    ASSERT_EQ(cb.empty(), true);
    ASSERT_EQ(cb.size(), 0);
    cb.push_back(1);
    ASSERT_EQ(cb.empty(), false);
    cb.clear();
    ASSERT_EQ(cb.empty(), true);
}

TEST(Other, At) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ('c', cb.at(1));
    cb.at(1) = 'b';
    ASSERT_EQ('b', cb.at(1));
    ASSERT_EQ('b', cb.back());
} 

TEST(Other, FrontAndBack) {
    CircularBuffer cb(2, 'c');
    cb.front() = 'a';
    cb.back() = 'b';
    ASSERT_EQ('a', cb.front());
    ASSERT_EQ('b', cb.back());
    ASSERT_EQ('a', cb[0]);
    ASSERT_EQ('b', cb[1]);
}

TEST(Other, SizeAndEmptyAndClear) {
    CircularBuffer cb(2, 'c');
    ASSERT_EQ(cb.empty(), false);
    ASSERT_EQ(cb.size(), 2);
    cb.clear();
    ASSERT_EQ(cb.empty(), true);
    ASSERT_EQ(cb.size(), 0);
    cb.push_back(1);
    ASSERT_EQ(cb.empty(), false);
    ASSERT_EQ(cb.size(), 1);
    cb.clear();
    ASSERT_EQ(cb.empty(), true);
}
