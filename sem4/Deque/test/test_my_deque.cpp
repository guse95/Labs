#include <gtest/gtest.h>
#include "my_deque.h"
#include <algorithm>


using namespace my_container;

TEST(DequeTest, DefaultConstructor) {
    Deque<int> d;
    EXPECT_EQ(d.size(), 0);
    EXPECT_TRUE(d.empty());
}

TEST(DequeTest, InitializerListConstructor) {
    Deque<int> d = {1, 2, 3, 4, 5};
    EXPECT_EQ(d.size(), 5);
    EXPECT_FALSE(d.empty());
    
    auto it = d.begin();
    EXPECT_EQ(*it, 1);
    ++it;
    EXPECT_EQ(*it, 2);
}

TEST(DequeTest, CopyConstructor) {
    Deque<int> original = {1, 2, 3};
    Deque<int> copy(original);
    
    EXPECT_EQ(original.size(), copy.size());
    EXPECT_TRUE(std::equal(original.begin(), original.end(), copy.begin()));
}

TEST(DequeTest, MoveConstructor) {
    Deque<int> original = {1, 2, 3};
    Deque<int> moved(std::move(original));
    
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(original.empty());
}

TEST(DequeTest, SizeConstructor) {
    Deque<int> d(5);
    EXPECT_EQ(d.size(), 5);
}

TEST(DequeTest, SizeValueConstructor) {
    Deque<int> d(5ULL, 10);
    EXPECT_EQ(d.size(), 5);
    for (const auto& item : d) {
        EXPECT_EQ(item, 10);
    }
}

TEST(DequeTest, AssignmentOperator) {
    Deque<int> original = {1, 2, 3};
    Deque<int> copy;
    copy = original;
    
    EXPECT_EQ(original.size(), copy.size());
    EXPECT_TRUE(std::equal(original.begin(), original.end(), copy.begin()));
}

TEST(DequeTest, MoveAssignmentOperator) {
    Deque<int> original = {1, 2, 3};
    Deque<int> moved;
    moved = std::move(original);
    
    EXPECT_EQ(moved.size(), 3);
    EXPECT_TRUE(original.empty());
}

TEST(DequeTest, Swap) {
    Deque<int> a = {1, 2, 3};
    Deque<int> b = {4, 5, 6, 7};
    
    a.swap(b);
    
    EXPECT_EQ(a.size(), 4);
    EXPECT_EQ(b.size(), 3);
    EXPECT_EQ(*a.begin(), 4);
    EXPECT_EQ(*b.begin(), 1);
}

TEST(DequeTest, FrontAndBack) {
    Deque<int> d = {10, 20, 30};
    EXPECT_EQ(d.front(), 10);
    EXPECT_EQ(d.back(), 30);
}

TEST(DequeTest, ConstFrontAndBack) {
    const Deque<int> d = {10, 20, 30};
    EXPECT_EQ(d.front(), 10);
    EXPECT_EQ(d.back(), 30);
}

TEST(DequeTest, Iterators) {
    Deque<int> d = {1, 2, 3, 4, 5};
    
    int sum = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
    
    sum = 0;
    for (const auto& item : d) {
        sum += item;
    }
    EXPECT_EQ(sum, 15);
}

TEST(DequeTest, ConstIterators) {
    const Deque<int> d = {1, 2, 3, 4, 5};
    
    int sum = 0;
    for (auto it = d.begin(); it != d.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST(DequeTest, ReverseIterators) {
    Deque<int> d = {1, 2, 3, 4, 5};
    
    std::vector<int> reversed;
    for (auto it = d.rbegin(); it != d.rend(); ++it) {
        reversed.push_back(*it);
    }
    
    std::vector<int> expected = {5, 4, 3, 2, 1};
    EXPECT_EQ(reversed, expected);
}

TEST(DequeTest, Empty) {
    Deque<int> d;
    EXPECT_TRUE(d.empty());
    
    d = {1};
    EXPECT_FALSE(d.empty());
}

TEST(DequeTest, Size) {
    Deque<int> d;
    EXPECT_EQ(d.size(), 0);
    
    d = {1, 2, 3};
    EXPECT_EQ(d.size(), 3);
}

TEST(DequeTest, MaxSize) {
    Deque<int> d;
    EXPECT_GT(d.max_size(), 0);
}

TEST(DequeTest, EqualityOperator) {
    Deque<int> a = {1, 2, 3};
    Deque<int> b = {1, 2, 3};
    Deque<int> c = {1, 2, 4};
    
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a == c);
}

TEST(DequeTest, InequalityOperator) {
    Deque<int> a = {1, 2, 3};
    Deque<int> b = {1, 2, 3};
    Deque<int> c = {1, 2, 4};
    
    EXPECT_FALSE(a != b);
    EXPECT_TRUE(a != c);
}

TEST(DequeTest, Clear) {
    Deque<int> d = {1, 2, 3};
    d.clear();
    
    EXPECT_TRUE(d.empty());
    EXPECT_EQ(d.size(), 0);
}

TEST(DequeTest, PushBack) {
    Deque<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(d.back(), 3);
}

TEST(DequeTest, PushFront) {
    Deque<int> d;
    d.push_front(1);
    d.push_front(2);
    d.push_front(3);
    
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(d.front(), 3);
}

TEST(DequeTest, PopBack) {
    Deque<int> d = {1, 2, 3};
    d.pop_back();
    
    EXPECT_EQ(d.size(), 2);
    EXPECT_EQ(d.back(), 2);
}

TEST(DequeTest, PopFront) {
    Deque<int> d = {1, 2, 3};
    d.pop_front();
    
    EXPECT_EQ(d.size(), 2);
    EXPECT_EQ(d.front(), 2);
}

TEST(DequeTest, Resize) {
    Deque<int> d = {1, 2, 3};
    d.resize(5);
    
    EXPECT_EQ(d.size(), 5);
    EXPECT_EQ(d.back(), 0);
    
    d.resize(2);
    EXPECT_EQ(d.size(), 2);
    EXPECT_EQ(d.back(), 2);
}

TEST(DequeTest, ResizeWithValue) {
    Deque<int> d = {1, 2, 3};
    d.resize(5, 10);
    
    EXPECT_EQ(d.size(), 5);
    EXPECT_EQ(d.back(), 10);
}

TEST(DequeTest, Insert) {
    Deque<int> d = {1, 3, 4};
    auto it = d.begin();
    ++it;
    d.insert(it, 2);
    
    EXPECT_EQ(d.size(), 4);
    EXPECT_EQ(d[1], 2);
}

TEST(DequeTest, Erase) {
    Deque<int> d = {1, 2, 3, 4};
    auto it = d.begin();
    ++it;
    d.erase(it);
    
    EXPECT_EQ(d.size(), 3);
    EXPECT_EQ(d[1], 3);
}

TEST(DequeTest, RandomAccess) {
    Deque<int> d = {1, 2, 3, 4, 5};
    
    EXPECT_EQ(d[0], 1);
    EXPECT_EQ(d[2], 3);
    EXPECT_EQ(d[4], 5);
    
    d[1] = 10;
    EXPECT_EQ(d[1], 10);
}

TEST(DequeTest, At) {
    Deque<int> d = {1, 2, 3};
    
    EXPECT_EQ(d.at(0), 1);
    EXPECT_EQ(d.at(2), 3);
    EXPECT_THROW(d.at(3), std::out_of_range);
}

TEST(DequeTest, StressTest) {
    Deque<int> d;
    const int N = 10000;
    
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            d.push_back(i);
        } else {
            d.push_front(i);
        }
    }
    
    EXPECT_EQ(d.size(), N);
    
    int sum = 0;
    for (const auto& item : d) {
        sum += item;
    }
    
    int expected_sum = ((N - 1) - 0) * (N/2);
    EXPECT_EQ(sum, expected_sum);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}