#include <gtest/gtest.h>
#include "my_stack.h"
#include <vector>
#include <algorithm>

using namespace my_container;

TEST(StackTest, DefaultConstructor) {
    Stack<int> s;
    EXPECT_TRUE(s.empty());
    EXPECT_EQ(s.size(), 0);
}

TEST(StackTest, DequeConstructor) {
    Deque<int> d = {1, 2, 3};
    Stack<int> s(d);
    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 3);
}

TEST(StackTest, MoveDequeConstructor) {
    Deque<int> d = {1, 2, 3};
    Stack<int> s(std::move(d));
    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 3);
    EXPECT_TRUE(d.empty());
}

TEST(StackTest, CopyConstructor) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2(s1);
    EXPECT_EQ(s1.size(), 3);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s1.top(), s2.top());
}

TEST(StackTest, MoveConstructor) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2(std::move(s1));
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s2.top(), 3);
    EXPECT_TRUE(s1.empty());
}

TEST(StackTest, InitializerListConstructor) {
    Stack<int> s = {1, 2, 3, 4, 5};
    EXPECT_EQ(s.size(), 5);
    EXPECT_EQ(s.top(), 5);
}

TEST(StackTest, IteratorConstructor) {
    std::vector<int> v = {1, 2, 3, 4};
    Stack<int> s(v.begin(), v.end());
    Stack<int> s_emp(v.begin(), v.begin());
    EXPECT_EQ(s.size(), 4);
    EXPECT_EQ(s.top(), 4);
    EXPECT_TRUE(s_emp.empty());
}

TEST(StackTest, IteratorConstructorWithAllocator) {
    std::vector<double> l = {1.1, 2.2, 3.3};
    Stack<double> s(l.begin(), l.end(), std::allocator<double>());

    EXPECT_EQ(s.size(), 3);
    EXPECT_DOUBLE_EQ(s.top(), 3.3);
}

TEST(StackTest, DequeCopyConstructorWithAllocator) {
    const Deque<int> d = {10, 20, 30};
    Stack<int> s(d, std::allocator<int>());

    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 30);
    EXPECT_EQ(d.size(), 3);
}

TEST(StackTest, DequeMoveConstructorWithAllocator) {
    Deque<int> d = {100, 200, 300};
    Stack<int> s(std::move(d), std::allocator<int>());

    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), 300);
    EXPECT_TRUE(d.empty());
}

TEST(StackTest, CopyConstructorWithAllocator) {
    Stack<int> original = {5, 10, 15};
    Stack<int> copy(original, std::allocator<int>());

    EXPECT_EQ(original.size(), 3);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(original.top(), copy.top());
}

TEST(StackTest, MoveConstructorWithAllocator) {
    Stack<int> original = {50, 100};
    Stack<int> moved(std::move(original), std::allocator<int>());

    EXPECT_EQ(moved.size(), 2);
    EXPECT_EQ(moved.top(), 100);
    EXPECT_TRUE(original.empty());
}

TEST(StackTest, InitializerListConstructorWithAllocator) {
    Stack<std::string> s(
        {"one", "two", "three"},
        std::allocator<std::string>()
    );

    EXPECT_EQ(s.size(), 3);
    EXPECT_EQ(s.top(), "three");
}

TEST(StackTest, CopyAssignment) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2;
    s2 = s1;
    EXPECT_EQ(s1.size(), 3);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s1.top(), s2.top());
}

TEST(StackTest, MoveAssignment) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2;
    s2 = std::move(s1);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s2.top(), 3);
    EXPECT_TRUE(s1.empty());
}

TEST(StackTest, Top) {
    Stack<int> s = {10, 20, 30};
    EXPECT_EQ(s.top(), 30);
    s.top() = 40;
    EXPECT_EQ(s.top(), 40);
}

TEST(StackTest, ConstTop) {
    const Stack<int> s = {10, 20, 30};
    EXPECT_EQ(s.top(), 30);
}

TEST(StackTest, Empty) {
    Stack<int> s;
    EXPECT_TRUE(s.empty());
    s.push(1);
    EXPECT_FALSE(s.empty());
}

TEST(StackTest, Size) {
    Stack<int> s;
    EXPECT_EQ(s.size(), 0);
    s.push(1);
    s.push(2);
    EXPECT_EQ(s.size(), 2);
}

TEST(StackTest, PushLValue) {
    Stack<int> s;
    int val = 42;
    s.push(val);
    EXPECT_EQ(s.top(), 42);
}

TEST(StackTest, PushRValue) {
    Stack<std::string> s;
    s.push(std::string("test"));
    EXPECT_EQ(s.top(), "test");
}

TEST(StackTest, Pop) {
    Stack<int> s = {1, 2, 3};
    s.pop();
    EXPECT_EQ(s.size(), 2);
    EXPECT_EQ(s.top(), 2);
}

TEST(StackTest, Swap) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2 = {4, 5};
    s1.swap(s2);
    EXPECT_EQ(s1.size(), 2);
    EXPECT_EQ(s2.size(), 3);
    EXPECT_EQ(s1.top(), 5);
    EXPECT_EQ(s2.top(), 3);
}

TEST(StackTest, EqualityOperator) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2 = {1, 2, 3};
    Stack<int> s3 = {1, 2};
    EXPECT_TRUE(s1 == s2);
    EXPECT_FALSE(s1 == s3);
}

TEST(StackTest, InequalityOperator) {
    Stack<int> s1 = {1, 2, 3};
    Stack<int> s2 = {1, 2, 4};
    EXPECT_TRUE(s1 != s2);
}

TEST(StackTest, ComparisonOperators) {
    Stack<int> s1 = {1, 2};
    Stack<int> s2 = {1, 2, 3};
    EXPECT_TRUE(s1 < s2);
    EXPECT_TRUE(s2 > s1);
    EXPECT_TRUE(s1 <= s2);
    EXPECT_TRUE(s2 >= s1);
}

#if __cplusplus >= 202002L
TEST(StackTest, ThreeWayComparison) {
    Stack<int> s1 = {1, 2};
    Stack<int> s2 = {1, 2, 3};
    EXPECT_TRUE((s1 <=> s2) < 0);
    EXPECT_TRUE((s2 <=> s1) > 0);
    EXPECT_TRUE((s1 <=> s1) == 0);
}
#endif

TEST(StackTest, StressTest) {
    Stack<int> s;
    const int N = 100000;
    for (int i = 0; i < N; ++i) {
        s.push(i);
    }
    EXPECT_EQ(s.size(), N);
    EXPECT_EQ(s.top(), N-1);

    for (int i = N-1; i >= 0; --i) {
        EXPECT_EQ(s.top(), i);
        s.pop();
    }
    EXPECT_TRUE(s.empty());
}

TEST(StackTest, Emplace) {
    Stack<std::pair<int, std::string>> s;
    s.push({1, "one"});
    EXPECT_EQ(s.top().first, 1);
    EXPECT_EQ(s.top().second, "one");
}

TEST(StackTest, Clear) {
    Stack<int> s = {1, 2, 3};
    while (!s.empty()) {
        s.pop();
    }
    EXPECT_TRUE(s.empty());
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}