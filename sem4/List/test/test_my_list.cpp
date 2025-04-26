#include <gtest/gtest.h>
#include "my_list.h"
#include <algorithm>
#include <vector>
#include <list>

using namespace my_container;

TEST(ListTest, DefaultConstructor) {
    List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST(ListTest, SizeConstructor) {
    List<int> l(5);
    EXPECT_EQ(l.size(), 5);
}

TEST(ListTest, SizeValueConstructor) {
    List<int> l(5ULL, 10);
    EXPECT_EQ(l.size(), 5);
    for (const auto& item : l) {
        EXPECT_EQ(item, 10);
    }
}

TEST(ListTest, InitializerListConstructor) {
    List<int> l = {1, 2, 3, 4, 5};
    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.front(), 1);
    EXPECT_EQ(l.back(), 5);
}

TEST(ListTest, CopyConstructor) {
    List<int> l1 = {1, 2, 3};
    List<int> l2(l1);
    EXPECT_EQ(l1.size(), l2.size());
    EXPECT_EQ(l1.front(), l2.front());
    EXPECT_EQ(l1.back(), l2.back());
}

TEST(ListTest, MoveConstructor) {
    List<int> l1 = {1, 2, 3};
    List<int> l2(std::move(l1));
    EXPECT_EQ(l2.size(), 3);
    EXPECT_TRUE(l1.empty());
}

TEST(ListTest, IteratorConstructor) {
    std::vector<int> v = {1, 2, 3, 4};
    List<int> l(v.begin(), v.end());
    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(l.back(), 4);
}

TEST(ListTest, AssignmentOperator) {
    List<int> l1 = {1, 2, 3};
    List<int> l2;
    l2 = l1;
    EXPECT_EQ(l1.size(), l2.size());
    EXPECT_EQ(l1.front(), l2.front());
}

TEST(ListTest, MoveAssignmentOperator) {
    List<int> l1 = {1, 2, 3};
    List<int> l2;
    l2 = std::move(l1);
    EXPECT_EQ(l2.size(), 3);
    EXPECT_TRUE(l1.empty());
}

TEST(ListTest, InitializerListAssignment) {
    List<int> l = {1, 2, 3, 4};
    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(l.back(), 4);
}

TEST(ListTest, FrontAndBack) {
    List<int> l = {10, 20, 30};
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 30);

    l.front() = 100;
    l.back() = 300;
    EXPECT_EQ(l.front(), 100);
    EXPECT_EQ(l.back(), 300);
}

TEST(ListTest, ConstFrontAndBack) {
    const List<int> l = {10, 20, 30};
    EXPECT_EQ(l.front(), 10);
    EXPECT_EQ(l.back(), 30);
}

TEST(ListTest, EmptyAndSize) {
    List<int> l;
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);

    l.push_back(1);
    EXPECT_FALSE(l.empty());
    EXPECT_EQ(l.size(), 1);
}

TEST(ListTest, MaxSize) {
    List<int> l;
    EXPECT_GT(l.max_size(), 0);
}

TEST(ListTest, Clear) {
    List<int> l = {1, 2, 3};
    l.clear();
    EXPECT_TRUE(l.empty());
    EXPECT_EQ(l.size(), 0);
}

TEST(ListTest, PushBack) {
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.back(), 2);
}

TEST(ListTest, PushBackRvalue) {
    List<std::string> l;
    l.push_back(std::string("test"));
    EXPECT_EQ(l.back(), "test");
}

TEST(ListTest, PushFront) {
    List<int> l;
    l.push_front(1);
    l.push_front(2);
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 2);
}

TEST(ListTest, PushFrontRvalue) {
    List<std::string> l;
    l.push_front(std::string("test"));
    EXPECT_EQ(l.front(), "test");
}

TEST(ListTest, PopBack) {
    List<int> l = {1, 2, 3};
    l.pop_back();
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.back(), 2);
}

TEST(ListTest, PopFront) {
    List<int> l = {1, 2, 3};
    l.pop_front();
    EXPECT_EQ(l.size(), 2);
    EXPECT_EQ(l.front(), 2);
}

TEST(ListTest, InsertSingleValue) {
    List<int> l = {1, 3, 4};
    auto it = l.begin();
    ++it;
    l.insert(it, 2);
    EXPECT_EQ(l.size(), 4);

    std::vector<int> expected = {1, 2, 3, 4};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), expected.begin()));
}

TEST(ListTest, InsertMultipleValues) {
    List<int> l = {1, 5};
    auto it = l.begin();
    ++it;
    l.insert(it, 3ULL, 3);
    EXPECT_EQ(l.size(), 5);

    std::vector<int> expected = {1, 3, 3, 3, 5};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), expected.begin()));
}

TEST(ListTest, InsertRange) {
    List<int> l = {1, 5};
    std::vector<int> v = {2, 3, 4};
    auto it = l.begin();
    ++it;
    l.insert(it, v.begin(), v.end());
    EXPECT_EQ(l.size(), 5);

    std::vector<int> expected = {1, 2, 3, 4, 5};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), expected.begin()));
}

TEST(ListTest, EraseSingle) {
    List<int> l = {1, 2, 3, 4};
    auto it = l.begin();
    ++it;
    l.erase(it);
    EXPECT_EQ(l.size(), 3);

    std::vector<int> expected = {1, 3, 4};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), expected.begin()));
}

TEST(ListTest, EraseRange) {
    List<int> l = {1, 2, 3, 4, 5};
    auto first = l.begin();
    ++first;
    auto last = first;
    ++last; ++last;
    l.erase(first, last);
    EXPECT_EQ(l.size(), 3);

    std::vector<int> expected = {1, 4, 5};
    EXPECT_TRUE(std::equal(l.begin(), l.end(), expected.begin()));
}

TEST(ListTest, ResizeIncrease) {
    List<int> l = {1, 2, 3};
    l.resize(5);
    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.back(), 0);
}

TEST(ListTest, ResizeDecrease) {
    List<int> l = {1, 2, 3, 4, 5};
    l.resize(3);
    EXPECT_EQ(l.size(), 3);
    EXPECT_EQ(l.back(), 3);
}

TEST(ListTest, ResizeWithValue) {
    List<int> l = {1, 2, 3};
    l.resize(5, 10);
    EXPECT_EQ(l.size(), 5);
    EXPECT_EQ(l.back(), 10);
}

TEST(ListTest, Swap) {
    List<int> l1 = {1, 2, 3};
    List<int> l2 = {4, 5};
    l1.swap(l2);
    EXPECT_EQ(l1.size(), 2);
    EXPECT_EQ(l2.size(), 3);
    EXPECT_EQ(l1.front(), 4);
    EXPECT_EQ(l2.front(), 1);
}

TEST(ListTest, Iterators) {
    List<int> l = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST(ListTest, ConstIterators) {
    const List<int> l = {1, 2, 3, 4, 5};
    int sum = 0;
    for (auto it = l.begin(); it != l.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 15);
}

TEST(ListTest, ReverseIterators) {
    List<int> l = {1, 2, 3, 4, 5};
    std::vector<int> reversed;
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        reversed.push_back(*it);
    }
    std::vector<int> expected = {5, 4, 3, 2, 1};
    EXPECT_EQ(reversed, expected);
}

TEST(ListTest, ComparisonOperators) {
    List<int> l1 = {1, 2, 3};
    List<int> l2 = {1, 2, 4};
    List<int> l3 = {1, 2, 3};

    EXPECT_TRUE(l1 == l3);
    EXPECT_TRUE(l1 != l2);
    EXPECT_TRUE(l1 < l2);
    EXPECT_TRUE(l2 > l1);
    EXPECT_TRUE(l1 <= l3);
    EXPECT_TRUE(l3 >= l1);
}

// #if __cplusplus >= 202002L
// TEST(ListTest, ThreeWayComparison) {
//     const List<int> l1 = {1, 2};
//     List<int> l2 = {1, 2, 3};
//     EXPECT_TRUE((l1 <=> l2) < 0);
//     EXPECT_TRUE((l2 <=> l1) > 0);
//     EXPECT_TRUE((l1 <=> l1) == 0);
// }
// #endif

TEST(ListTest, StressTest) {
    List<int> l;
    const int N = 100000;
    for (int i = 0; i < N; ++i) {
        if (i % 2 == 0) {
            l.push_back(i);
        } else {
            l.push_front(i);
        }
    }
    EXPECT_EQ(l.size(), N);

    int sum = 0;
    for (const auto& item : l) {
        sum += item;
    }
    EXPECT_GT(sum, 0);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}