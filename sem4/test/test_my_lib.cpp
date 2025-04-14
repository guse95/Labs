#include <gtest/gtest.h>
#include "my_lib.h"

class TestFoo : public ::testing::Test {
};


namespace my_container::testing {
    class ArrayTest : public ::testing::Test {
    protected:
        void SetUp() override {
            for(int i = 0; i < 5; ++i) {
                testArray[i] = i + 1;
            }
        }

        Array<int, 5> emptyArray;
        Array<int, 5> testArray;
    };

    TEST_F(ArrayTest, DefaultConstructor) {
        EXPECT_EQ(emptyArray.size(), 5);
        EXPECT_FALSE(emptyArray.empty());
    }

    TEST_F(ArrayTest, InitializerListConstructor) {
        Array<int, 5> arr{1, 2, 3, 4, 5};
        EXPECT_EQ(arr.size(), 5);
        for(int i = 0; i < 5; ++i) {
            EXPECT_EQ(arr[i], i + 1);
        }
    }

    TEST_F(ArrayTest, PairInitializerListConstructor) {
        Array<int, 5> arr{{0, 10}, {2, 30}, {4, 50}};
        EXPECT_EQ(arr[0], 10);
        EXPECT_EQ(arr[2], 30);
        EXPECT_EQ(arr[4], 50);
    }

    TEST_F(ArrayTest, CopyConstructor) {
        Array<int, 5> copy(testArray);
        EXPECT_EQ(copy.size(), testArray.size());
        for(int i = 0; i < 5; ++i) {
            EXPECT_EQ(copy[i], testArray[i]);
        }
    }

    TEST_F(ArrayTest, MoveConstructor) {
        Array<int, 5> temp(testArray);
        Array<int, 5> moved(std::move(temp));

        EXPECT_EQ(moved.size(), 5);
        EXPECT_EQ(temp.size(), 0);
        EXPECT_EQ(temp.data(), nullptr);

        for(int i = 0; i < 5; ++i) {
            EXPECT_EQ(moved[i], i + 1);
        }
    }


    TEST_F(ArrayTest, AssignmentOperator) {
        Array<int, 5> copy;
        copy = testArray;

        for(int i = 0; i < 5; ++i) {
            EXPECT_EQ(copy[i], testArray[i]);
        }
    }

    TEST_F(ArrayTest, MoveAssignmentOperator) {
        Array<int, 5> temp(testArray);
        Array<int, 5> moved;
        moved = std::move(temp);

        EXPECT_EQ(moved.size(), 5);
        EXPECT_EQ(temp.size(), 0);
        EXPECT_EQ(temp.data(), nullptr);
    }

    TEST_F(ArrayTest, ComparisonOperators) {
        const Array<int, 5> same{1, 2, 3, 4, 5};
        const Array<int, 5> less{0, 2, 3, 4, 5};
        const Array<int, 5> greater{2, 2, 3, 4, 5};

        EXPECT_TRUE(testArray == same);
        EXPECT_TRUE(testArray != less);
        EXPECT_TRUE(less < testArray);
        EXPECT_TRUE(greater > testArray);
        EXPECT_TRUE(less <= testArray);
        EXPECT_TRUE(testArray >= less);
    }

    TEST_F(ArrayTest, AtMethod) {
        EXPECT_EQ(testArray.at(2), 3);
        EXPECT_THROW(testArray.at(5), std::out_of_range);
    }

    TEST_F(ArrayTest, SubscriptOperator) {
        EXPECT_EQ(testArray[2], 3);
        testArray[2] = 10;
        EXPECT_EQ(testArray[2], 10);
    }

    TEST_F(ArrayTest, FrontBackMethods) {
        EXPECT_EQ(testArray.front(), 1);
        EXPECT_EQ(testArray.back(), 5);

        testArray.front() = 10;
        testArray.back() = 20;
        EXPECT_EQ(testArray.front(), 10);
        EXPECT_EQ(testArray.back(), 20);
    }

    TEST_F(ArrayTest, DataMethod) {
        int* ptr = testArray.data();
        EXPECT_EQ(ptr[0], 1);
        ptr[0] = 10;
        EXPECT_EQ(testArray.front(), 10);
    }

    TEST_F(ArrayTest, ForwardIteration) {
        int expected = 1;
        for(auto it = testArray.begin(); it != testArray.end(); ++it) {
            EXPECT_EQ(*it, expected++);
        }
    }

    TEST_F(ArrayTest, ReverseIteration) {
        int expected = 5;
        for(auto rit = testArray.rbegin(); rit != testArray.rend(); ++rit) {
            EXPECT_EQ(*rit, expected--);
        }
    }

    TEST_F(ArrayTest, ConstIterators) {
        const Array<int, 5>& constRef = testArray;
        int expected = 1;
        for(auto it = constRef.cbegin(); it != constRef.cend(); ++it) {
            EXPECT_EQ(*it, expected++);
        }
    }

    TEST_F(ArrayTest, SizeMethods) {
        EXPECT_EQ(testArray.size(), 5);
        EXPECT_EQ(testArray.max_size(), 5);
        EXPECT_FALSE(testArray.empty());
    }

    TEST_F(ArrayTest, FillMethod) {
        testArray.fill(42);
        for(auto&& item : testArray) {
            EXPECT_EQ(item, 42);
        }
    }

    TEST_F(ArrayTest, SwapMethod) {
        Array<int, 5> other;
        other.fill(10);

        testArray.swap(other);

        for(auto&& item : testArray) {
            EXPECT_EQ(item, 10);
        }

        int expected = 1;
        for(auto&& item : other) {
            EXPECT_EQ(item, expected++);
        }
    }

    TEST_F(ArrayTest, ExceptionSafety) {
        Array<int, 5> arr;
        EXPECT_NO_THROW(arr.at(0));
        EXPECT_NO_THROW(arr[0]);
        EXPECT_THROW(arr.at(5), std::out_of_range);

        const Array<int, 5>& constArr = arr;
        EXPECT_NO_THROW(constArr.at(0));
        EXPECT_THROW(constArr.at(5), std::out_of_range);
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
