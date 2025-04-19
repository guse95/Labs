#include <gtest/gtest.h>
#include "my_list.h"

class TestFoo : public ::testing::Test {
};


namespace my_container::testing {

    class ListTest : public ::testing::Test {
    protected:
        void SetUp() override {
            for(int i = 1; i <= 5; ++i) {
                testList.push_back(i);
            }
        }

        List<int> emptyList;
        List<int> testList;
    };

    // Тестирование конструкторов
    TEST_F(ListTest, DefaultConstructor) {
        EXPECT_TRUE(emptyList.empty());
        EXPECT_EQ(emptyList.size(), 0);
    }

    TEST_F(ListTest, SizeConstructor) {
        List<int> list(5);
        EXPECT_EQ(list.size(), 5);
    }

    TEST_F(ListTest, SizeValueConstructor) {
        List<int> list(5ULL, 42);
        EXPECT_EQ(list.size(), 5);
        for(auto&& item : list) {
            EXPECT_EQ(item, 42);
        }
    }

    TEST_F(ListTest, InitializerListConstructor) {
        List<int> list{1, 2, 3};
        EXPECT_EQ(list.size(), 3);
        EXPECT_EQ(list.front(), 1);
        EXPECT_EQ(list.back(), 3);
    }

    TEST_F(ListTest, CopyConstructor) {
        List<int> copy(testList);
        EXPECT_EQ(copy.size(), testList.size());

        auto it1 = testList.begin();
        auto it2 = copy.begin();
        while(it1 != testList.end() && it2 != copy.end()) {
            EXPECT_EQ(*it1, *it2);
            ++it1;
            ++it2;
        }
    }

    TEST_F(ListTest, MoveConstructor) {
        List<int> temp(testList);
        List<int> moved(std::move(temp));

        EXPECT_EQ(moved.size(), 5);
        EXPECT_EQ(temp.size(), 0);
        EXPECT_TRUE(temp.empty());

        int expected = 1;
        for(auto&& item : moved) {
            EXPECT_EQ(item, expected++);
        }
    }

    // Тестирование операторов
    TEST_F(ListTest, AssignmentOperator) {
        List<int> copy;
        copy = testList;

        EXPECT_EQ(copy.size(), testList.size());

        auto it1 = testList.begin();
        auto it2 = copy.begin();
        while(it1 != testList.end() && it2 != copy.end()) {
            EXPECT_EQ(*it1, *it2);
            ++it1;
            ++it2;
        }
    }

    TEST_F(ListTest, MoveAssignmentOperator) {
        List<int> temp(testList);
        List<int> moved;
        moved = std::move(temp);

        EXPECT_EQ(moved.size(), 5);
        EXPECT_EQ(temp.size(), 0);
        EXPECT_TRUE(temp.empty());

        int expected = 1;
        for(auto&& item : moved) {
            EXPECT_EQ(item, expected++);
        }
    }

    TEST_F(ListTest, ComparisonOperators) {
        List<int> same{1, 2, 3, 4, 5};
        List<int> less{0, 2, 3, 4, 5};
        List<int> greater{2, 2, 3, 4, 5};

        EXPECT_TRUE(testList == same);
        EXPECT_TRUE(testList != less);
        EXPECT_TRUE(less < testList);
        EXPECT_TRUE(greater > testList);
        EXPECT_TRUE(less <= testList);
        EXPECT_TRUE(testList >= less);
    }

    // Тестирование методов доступа
    TEST_F(ListTest, FrontBackMethods) {
        EXPECT_EQ(testList.front(), 1);
        EXPECT_EQ(testList.back(), 5);

        testList.front() = 10;
        testList.back() = 20;
        EXPECT_EQ(testList.front(), 10);
        EXPECT_EQ(testList.back(), 20);
    }

    // Тестирование итераторов
    TEST_F(ListTest, ForwardIteration) {
        int expected = 1;
        for(auto it = testList.begin(); it != testList.end(); ++it) {
            EXPECT_EQ(*it, expected++);
        }
    }

    TEST_F(ListTest, ReverseIteration) {
        int expected = 5;
        for(auto rit = testList.rbegin(); rit != testList.rend(); ++rit) {
            EXPECT_EQ(*rit, expected--);
        }
    }

    TEST_F(ListTest, ConstIterators) {
        List<int>& constRef = testList;
        int expected = 1;
        for(auto it = constRef.begin(); it != constRef.end(); ++it) {
            EXPECT_EQ(*it, expected++);
        }
    }

    // Тестирование методов размера
    TEST_F(ListTest, SizeMethods) {
        EXPECT_EQ(testList.size(), 5);
        EXPECT_GT(testList.max_size(), 0);
        EXPECT_FALSE(testList.empty());

        emptyList.clear();
        EXPECT_TRUE(emptyList.empty());
    }

    // Тестирование модификаторов
    TEST_F(ListTest, PushPopMethods) {
        testList.push_front(0);
        EXPECT_EQ(testList.front(), 0);
        EXPECT_EQ(testList.size(), 6);

        testList.pop_front();
        EXPECT_EQ(testList.front(), 1);
        EXPECT_EQ(testList.size(), 5);

        testList.push_back(6);
        EXPECT_EQ(testList.back(), 6);
        EXPECT_EQ(testList.size(), 6);

        testList.pop_back();
        EXPECT_EQ(testList.back(), 5);
        EXPECT_EQ(testList.size(), 5);
    }

    TEST_F(ListTest, InsertEraseMethods) {
        auto it = testList.begin();
        ++it; // points to 2

        testList.insert(it, 10);
        EXPECT_EQ(testList.size(), 6);

        it = testList.begin();
        ++it; // points to 10
        EXPECT_EQ(*it, 10);

        it = testList.erase(it);
        EXPECT_EQ(*it, 2);
        EXPECT_EQ(testList.size(), 5);
    }

    TEST_F(ListTest, ResizeMethod) {
        testList.resize(3);
        EXPECT_EQ(testList.size(), 3);
        EXPECT_EQ(testList.back(), 3);

        testList.resize(5, 42);
        EXPECT_EQ(testList.size(), 5);
        EXPECT_EQ(testList.back(), 42);
    }

    TEST_F(ListTest, SwapMethod) {
        List<int> other {10, 20, 30};
        testList.swap(other);

        EXPECT_EQ(testList.size(), 3);
        EXPECT_EQ(other.size(), 5);

        EXPECT_EQ(testList.front(), 10);
        EXPECT_EQ(other.front(), 1);
    }

    // Тестирование исключений
    TEST_F(ListTest, ExceptionSafety) {
        EXPECT_THROW(emptyList.front(), std::out_of_range);
        EXPECT_THROW(emptyList.back(), std::out_of_range);

        List<int> list(1);
        EXPECT_NO_THROW(list.front());
        EXPECT_NO_THROW(list.back());
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
