#include <gtest/gtest.h>
#include "my_list.h"
#include <algorithm>
#include <vector>
#include <list>



namespace my_container::testing {

    class ListTest : public ::testing::Test {
    protected:
        List<int> empty_list;
        List<int> test_list{1, 2, 3};
        List<std::string> string_list{"a", "b", "c"};
    };

    TEST_F(ListTest, DefaultConstructor) {
        EXPECT_TRUE(empty_list.empty());
        EXPECT_EQ(empty_list.size(), 0);
    }

    TEST_F(ListTest, InitializerListConstructor) {
        EXPECT_EQ(test_list.size(), 3);
        EXPECT_EQ(test_list.front(), 1);
        EXPECT_EQ(test_list.back(), 3);
    }

    TEST_F(ListTest, CopyConstructor) {
        List<int> copy(test_list);
        EXPECT_EQ(copy.size(), 3);
        EXPECT_EQ(copy.front(), 1);
        EXPECT_EQ(copy.back(), 3);
    }

    TEST_F(ListTest, MoveConstructor) {
        List<int> moved(std::move(test_list));
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(test_list.empty());
    }

    TEST_F(ListTest, SizeConstructor) {
        List<int> list(5);
        EXPECT_EQ(list.size(), 5);
    }

    TEST_F(ListTest, SizeValueConstructor) {
        List<int> list(5ULL, 42);
        EXPECT_EQ(list.size(), 5);
        EXPECT_EQ(list.front(), 42);
        EXPECT_EQ(list.back(), 42);
    }

    TEST_F(ListTest, IteratorConstructor) {
        std::vector<int> vec{1, 2, 3};
        List<int> list(vec.begin(), vec.end());
        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST_F(ListTest, AssignmentOperator) {
        List<int> copy;
        copy = test_list;
        EXPECT_EQ(copy.size(), 3);
        EXPECT_EQ(copy.front(), 1);
    }

    TEST_F(ListTest, MoveAssignment) {
        List<int> moved;
        moved = std::move(test_list);
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(test_list.empty());
    }

    TEST_F(ListTest, InitializerListAssignment) {
        test_list = {4, 5, 6};
        EXPECT_EQ(test_list.size(), 3);
        EXPECT_EQ(test_list.front(), 4);
    }

    TEST_F(ListTest, FrontBackMethods) {
        EXPECT_EQ(test_list.front(), 1);
        EXPECT_EQ(test_list.back(), 3);
        EXPECT_THROW(empty_list.front(), std::out_of_range);
    }

    TEST_F(ListTest, BeginEndIterators) {
        auto it = test_list.begin();
        EXPECT_EQ(*it, 1);
        ++it;
        EXPECT_EQ(*it, 2);

        int sum = 0;
        for (const auto& item : test_list) {
            sum += item;
        }
        EXPECT_EQ(sum, 6);
    }

    TEST_F(ListTest, ConstIterators) {
        const auto& const_list = test_list;
        EXPECT_EQ(*const_list.begin(), 1);
        EXPECT_EQ(*const_list.cbegin(), 1);
    }

    TEST_F(ListTest, ReverseIterators) {
        auto rit = test_list.rbegin();
        EXPECT_EQ(*rit, 3);
        ++rit;
        EXPECT_EQ(*rit, 2);
    }

    TEST_F(ListTest, EmptySizeMaxSize) {
        EXPECT_TRUE(empty_list.empty());
        EXPECT_EQ(test_list.size(), 3);
        EXPECT_GT(test_list.max_size(), 0);
    }

    TEST_F(ListTest, Clear) {
        test_list.clear();
        EXPECT_TRUE(test_list.empty());
        EXPECT_EQ(test_list.size(), 0);
    }

    TEST_F(ListTest, InsertSingleElement) {
        auto it = test_list.begin();
        ++it;
        test_list.insert(it, 42);
        EXPECT_EQ(test_list.size(), 4);
        EXPECT_EQ(*++test_list.begin(), 42);
    }

    TEST_F(ListTest, InsertMultipleElements) {
        auto it = test_list.begin();
        test_list.insert(it, 3ULL, 42);
        EXPECT_EQ(test_list.size(), 6);
        EXPECT_EQ(test_list.front(), 42);
    }

    TEST_F(ListTest, InsertRange) {
        std::vector<int> vec{4, 5};
        auto it = test_list.end();
        test_list.insert(it, vec.begin(), vec.end());
        EXPECT_EQ(test_list.size(), 5);
        EXPECT_EQ(test_list.back(), 5);
    }

    TEST_F(ListTest, EraseSingleElement) {
        auto it = test_list.begin();
        it = test_list.erase(it);
        EXPECT_EQ(test_list.size(), 2);
        EXPECT_EQ(*it, 2);
    }

    TEST_F(ListTest, EraseRange) {
        auto first = test_list.begin();
        auto last = test_list.begin();
        ++last; ++last;
        test_list.erase(first, last);
        EXPECT_EQ(test_list.size(), 1);
        EXPECT_EQ(test_list.front(), 3);
    }

    TEST_F(ListTest, PushBack) {
        test_list.push_back(4);
        EXPECT_EQ(test_list.size(), 4);
        EXPECT_EQ(test_list.back(), 4);
    }

    TEST_F(ListTest, PushBackMove) {
        std::string s = "test";
        string_list.push_back(std::move(s));
        EXPECT_EQ(string_list.back(), "test");
    }

    TEST_F(ListTest, PopBack) {
        test_list.pop_back();
        EXPECT_EQ(test_list.size(), 2);
        EXPECT_EQ(test_list.back(), 2);
        EXPECT_THROW(empty_list.pop_back(), std::out_of_range);
    }

    TEST_F(ListTest, PushFront) {
        test_list.push_front(0);
        EXPECT_EQ(test_list.size(), 4);
        EXPECT_EQ(test_list.front(), 0);
    }

    TEST_F(ListTest, PushFrontMove) {
        std::string s = "test";
        string_list.push_front(std::move(s));
        EXPECT_EQ(string_list.front(), "test");
        // EXPECT_TRUE(s.empty());
    }

    TEST_F(ListTest, PopFront) {
        test_list.pop_front();
        EXPECT_EQ(test_list.size(), 2);
        EXPECT_EQ(test_list.front(), 2);
        EXPECT_THROW(empty_list.pop_front(), std::out_of_range);
    }

    TEST_F(ListTest, Resize) {
        test_list.resize(5);
        EXPECT_EQ(test_list.size(), 5);
        test_list.resize(2);
        EXPECT_EQ(test_list.size(), 2);
    }

    TEST_F(ListTest, ResizeWithValue) {
        test_list.resize(5, 42);
        EXPECT_EQ(test_list.size(), 5);
        EXPECT_EQ(test_list.back(), 42);
    }

    TEST_F(ListTest, Swap) {
        List<int> other{4, 5};
        test_list.swap(other);
        EXPECT_EQ(test_list.size(), 2);
        EXPECT_EQ(other.size(), 3);
    }

    TEST_F(ListTest, ComparisonOperators) {
        List<int> same{1, 2, 3};
        List<int> smaller{1, 2, 2};
        List<int> larger{1, 2, 4};

        EXPECT_TRUE(test_list == same);
        EXPECT_TRUE(test_list != smaller);
        EXPECT_TRUE(test_list > smaller);
        EXPECT_TRUE(test_list < larger);
    }

    #if __cplusplus >= 202002L
    TEST_F(ListTest, ThreeWayComparison) {
        List<int> same{1, 2, 3};
        List<int> smaller{1, 2, 2};

        EXPECT_EQ(test_list <=> same, std::weak_ordering::equivalent);
        EXPECT_EQ(test_list <=> smaller, std::weak_ordering::greater);
    }
    #endif

    TEST_F(ListTest, IteratorOperations) {
        auto it1 = test_list.begin();
        auto it2 = it1++;
        EXPECT_EQ(*it2, 1);
        EXPECT_EQ(*it1, 2);

        --it1;
        EXPECT_EQ(*it1, 1);

        auto rit = test_list.rbegin();
        EXPECT_EQ(*rit, 3);
        ++rit;
        EXPECT_EQ(*rit, 2);
    }

    TEST_F(ListTest, IteratorDereferenceException) {
        List<int>::iterator it;
        EXPECT_THROW(*it, std::runtime_error);
    }

    TEST_F(ListTest, AllocatorAware) {
        std::allocator<int> alloc;
        List<int, std::allocator<int>> list(alloc);
        list.push_back(1);
        EXPECT_EQ(list.size(), 1);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
