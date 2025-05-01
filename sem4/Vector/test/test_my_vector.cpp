#include <gtest/gtest.h>
#include "my_vector.h"
#include <list>

using namespace my_container;

class VectorTest : public ::testing::Test {};

TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_EQ(v.size(), 0);
    EXPECT_EQ(v.capacity(), 0);
    EXPECT_TRUE(v.empty());
}

TEST_F(VectorTest, SizeConstructor) {
    Vector<int> v(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], int());
    }
}

TEST_F(VectorTest, SizeValueConstructor) {
    Vector<int> v(5ULL, 42);
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], 42);
    }
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> v{1, 2, 3, 4, 5};
    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST_F(VectorTest, IterConstructor) {
    std::list<int> ls{1, 2, 3, 4, 5};
    Vector<int> v(ls.begin(), ls.end());

    EXPECT_EQ(v.size(), 5);
    EXPECT_GE(v.capacity(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
    EXPECT_THROW(v[6], std::out_of_range);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original);

    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
    EXPECT_THROW(copy[6], std::out_of_range);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> original{1, 2, 3};
    Vector<int> moved(std::move(original));

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
    EXPECT_TRUE(original.empty());
}

TEST_F(VectorTest, CopyConstructorWithAlloc) {
    Vector<int> original{1, 2, 3};
    Vector<int> copy(original, std::allocator<long long int>());

    EXPECT_EQ(original.size(), copy.size());
    for (size_t i = 0; i < original.size(); ++i) {
        EXPECT_EQ(original[i], copy[i]);
    }
    EXPECT_THROW(copy[6], std::out_of_range);
}

TEST_F(VectorTest, MoveConstructorWithAlloc) {
    Vector<int> original{1, 2, 3};
    Vector<int> moved(std::move(original), std::allocator<long long int>());

    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(original.size(), 0);
    EXPECT_EQ(original.capacity(), 0);
    EXPECT_TRUE(original.empty());
}

TEST_F(VectorTest, AssignmentOperator) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    Vector<int> v3{4, 5, 6, 7};
    v2 = v1;
    v3 = v2;
    EXPECT_EQ(v1.size(), v2.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        EXPECT_EQ(v1[i], v2[i]);
    }

    EXPECT_EQ(v2.size(), v3.size());
    for (size_t i = 0; i < v2.size(); ++i) {
        EXPECT_EQ(v3[i], v2[i]);
    }
}

TEST_F(VectorTest, MoveAssignmentOperator) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2;
    v2 = std::move(v1);

    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v1.capacity(), 0);
    EXPECT_TRUE(v1.empty());
}

TEST_F(VectorTest, InitializerListAssignment) {
    Vector<int> v;
    v = {1, 2, 3, 4, 5};

    EXPECT_EQ(v.size(), 5);
    for (size_t i = 0; i < v.size(); ++i) {
        EXPECT_EQ(v[i], static_cast<int>(i + 1));
    }
}

TEST_F(VectorTest, ElementAccess) {
    Vector<int> v{10, 20, 30};

    EXPECT_EQ(v[0], 10);
    EXPECT_EQ(v[1], 20);
    EXPECT_EQ(v[2], 30);

    EXPECT_EQ(v.at(0), 10);
    EXPECT_EQ(v.at(1), 20);
    EXPECT_EQ(v.at(2), 30);

    EXPECT_THROW(v.at(3), std::out_of_range);
}

TEST_F(VectorTest, FrontBack) {
    Vector<int> v{10, 20, 30};

    const int a = v.front();
    EXPECT_EQ(a, 10);
    const int b = v.back();
    EXPECT_EQ(b, 30);

    EXPECT_EQ(v.front(), 10);
    EXPECT_EQ(v.back(), 30);

    v.clear();
    EXPECT_THROW(v.front(), std::out_of_range);
    EXPECT_THROW(v.back(), std::out_of_range);
}

TEST_F(VectorTest, Data) {
    Vector<int> v{10, 20, 30};
    int* data = v.data();

    EXPECT_EQ(data[0], 10);
    EXPECT_EQ(data[1], 20);
    EXPECT_EQ(data[2], 30);
}

TEST_F(VectorTest, Iterators) {
    Vector<int> v{1, 2, 3};

    // Forward iterators
    int expected = 1;
    for (auto it = v.begin(); it != v.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    // Const forward iterators
    expected = 1;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    // Reverse iterators
    expected = 3;
    for (auto it = v.rbegin(); it != v.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }

    // Const reverse iterators
    expected = 3;
    for (auto it = v.crbegin(); it != v.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

TEST_F(VectorTest, Reserve) {
    Vector<int> v;
    v.reserve(100);

    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 100);

    v = {1, 2, 3};
    v.reserve(50); // Shouldn't shrink
    EXPECT_GE(v.capacity(), 100);
}

TEST_F(VectorTest, ShrinkToFit) {
    Vector<int> v;
    v.reserve(100);
    v = {1, 2, 3};
    v.shrink_to_fit();

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v.capacity(), 3);
}

TEST_F(VectorTest, Clear) {
    Vector<int> v{1, 2, 3};
    v.clear();

    EXPECT_EQ(v.size(), 0);
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.capacity(), 3);
}

TEST_F(VectorTest, PushBack) {
    Vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);

    constexpr int a = 52;
    constexpr int b = 42;
    Vector<int> v1;
    v1.push_back(a);
    v1.push_back(b);
    v1.push_back(a);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    EXPECT_EQ(v1.size(), 3);
    EXPECT_EQ(v1[0], 52);
    EXPECT_EQ(v1[1], 42);
    EXPECT_EQ(v1[2], 52);

    // Test rvalue push_back
    int val = 4;
    v.push_back(std::move(val));
    EXPECT_EQ(v[3], 4);
}

TEST_F(VectorTest, PopBack) {
    Vector<int> v{1, 2, 3};
    v.pop_back();

    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v.back(), 2);

    v.pop_back();
    v.pop_back();
    EXPECT_THROW(v.pop_back(), std::out_of_range);
}

TEST_F(VectorTest, InsertSingleElement) {
    Vector<int> v;
    v.insert(v.begin(), 1);
    v.insert(v.begin() + 1, 2);
    v.insert(v.begin() + 2, 4);
    auto it = v.insert(v.begin() + 2, 3);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_THROW(v.insert(v.begin() + 7, 5), std::out_of_range);

    Vector<int> v1;
    int a = 42;
    int b = 52;
    v1.insert(v1.begin(), a);
    v1.insert(v1.begin() + 1, b);
    v1.insert(v1.begin() + 2, b);
    auto it1 = v1.insert(v1.begin() + 2, a);

    EXPECT_EQ(v1.size(), 4);
    EXPECT_EQ(*it1, 42);
    EXPECT_EQ(v1[0], 42);
    EXPECT_EQ(v1[1], 52);
    EXPECT_EQ(v1[2], 42);
    EXPECT_EQ(v1[3], 52);
    EXPECT_THROW(v1.insert(v1.begin() + 10, a), std::out_of_range);

    int val = 5;
    it = v.insert(v.end(), val);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(*it, 5);
}

TEST_F(VectorTest, InsertMultipleElements) {
    Vector<int> v;
    v.insert(v.begin(), 1ULL, 1);
    v.insert(v.end(), 1ULL, 5);
    v.insert(v.begin() + 1, 3ULL, 3);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 3);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 3);
    EXPECT_EQ(v[4], 5);

    Vector<int> v1;
    int a = 42;
    int b = 52;
    int c = 11;
    v1.insert(v1.begin(), 2ULL, a);
    v1.insert(v1.end(), 2ULL, b);
    v1.insert(v1.begin() + 1, 3ULL, c);

    EXPECT_EQ(v1.size(), 7);
    EXPECT_EQ(v1[0], 42);
    EXPECT_EQ(v1[1], 11);
    EXPECT_EQ(v1[2], 11);
    EXPECT_EQ(v1[3], 11);
    EXPECT_EQ(v1[4], 42);
    EXPECT_EQ(v1[5], 52);
    EXPECT_EQ(v1[6], 52);
}

TEST_F(VectorTest, InsertRange) {
    Vector<int> source;
    Vector<int> v0{2};
    Vector<int> v1{3, 4};
    Vector<int> v{1, 5};
    v1.insert(v1.begin(), v0.begin(), v0.end());
    source.insert(source.begin(), v1.begin(), v1.end());
    v.insert(v.begin() + 1, source.begin(), source.end());
    EXPECT_THROW(v.insert(v.begin() + 10, v1.begin(), v1.end()), std::out_of_range);

    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
}

TEST_F(VectorTest, InsertInitializerList) {
    Vector<int> v;
    v.insert(v.begin(), {1, 5});
    v.insert(v.end(), {6});
    v.insert(v.begin() + 1, {2, 3, 4});
    EXPECT_THROW(v.insert(v.begin() + 10, {2, 3, 4}), std::out_of_range);

    EXPECT_EQ(v.size(), 6);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 4);
    EXPECT_EQ(v[4], 5);
    EXPECT_EQ(v[5], 6);
}

TEST_F(VectorTest, EraseSingleElement) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 2);

    EXPECT_THROW(v.erase(v.begin() + 10), std::out_of_range);

    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 4);
    EXPECT_EQ(v[3], 5);
}

TEST_F(VectorTest, EraseRange) {
    Vector<int> v{1, 2, 3, 4, 5};
    auto it = v.erase(v.begin() + 1, v.begin() + 3);

    EXPECT_THROW(v.erase(v.begin() + 10, v.begin() + 15), std::out_of_range);

    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(*it, 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 4);
    EXPECT_EQ(v[2], 5);
}

TEST_F(VectorTest, Resize) {
    Vector<int> v{1, 2, 3};

    // Resize larger
    v.resize(5);
    EXPECT_EQ(v.size(), 5);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);

    v.resize(7, 52);
    EXPECT_EQ(v.size(), 7);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
    EXPECT_EQ(v[3], 0);
    EXPECT_EQ(v[4], 0);
    EXPECT_EQ(v[5], 52);
    EXPECT_EQ(v[6], 52);

    v.resize(3, 52);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);

    // Resize smaller
    v.resize(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);

    // Resize with value
    v.resize(4, 42);
    EXPECT_EQ(v.size(), 4);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 42);
    EXPECT_EQ(v[3], 42);
}

TEST_F(VectorTest, Swap) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{4, 5, 6, 7};

    v1.swap(v2);

    EXPECT_EQ(v1.size(), 4);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2};
    Vector<int> v4{1, 2, 4};

    EXPECT_TRUE(v1 == v2);
    EXPECT_FALSE(v1 == v3);
    EXPECT_FALSE(v1 == v4);

    EXPECT_FALSE(v1 != v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v1 != v4);

    EXPECT_FALSE(v1 < v2);
    EXPECT_FALSE(v1 < v3);
    EXPECT_TRUE(v3 < v1);
    EXPECT_TRUE(v1 < v4);

    EXPECT_TRUE(v1 <= v2);
    EXPECT_FALSE(v1 <= v3);
    EXPECT_TRUE(v3 <= v1);
    EXPECT_TRUE(v1 <= v4);

    EXPECT_FALSE(v1 > v2);
    EXPECT_TRUE(v1 > v3);
    EXPECT_FALSE(v3 > v1);
    EXPECT_FALSE(v1 > v4);

    EXPECT_TRUE(v1 >= v2);
    EXPECT_TRUE(v1 >= v3);
    EXPECT_FALSE(v3 >= v1);
    EXPECT_FALSE(v1 >= v4);
}

#if __cplusplus >= 202002L
TEST_F(VectorTest, ThreeWayComparison) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2};
    Vector<int> v4{1, 2, 4};

    EXPECT_EQ(v1 <=> v2, 0);
    EXPECT_GT((v1 <=> v3), 0);
    EXPECT_LT((v3 <=> v1), 0);
    EXPECT_LT((v1 <=> v4), 0);
    EXPECT_GT((v4 <=> v1), 0);
}
#endif

TEST_F(VectorTest, AllocatorAware) {
    std::allocator<int> alloc;
    Vector<int, std::allocator<int>> v1(alloc);
    Vector<int, std::allocator<int>> v2(5, alloc);
    Vector<int, std::allocator<int>> v3(5ULL, 42, alloc);

    EXPECT_EQ(v1.size(), 0);
    EXPECT_EQ(v2.size(), 5);
    EXPECT_EQ(v3.size(), 5);

    for (auto val : v3) {
        EXPECT_EQ(val, 42);
    }
}

TEST_F(VectorTest, IteratorOperators) {
    Vector<int> v{1, 2, 3};

    // Test prefix and postfix increment
    auto it = v.begin();
    EXPECT_EQ(*it, 1);
    EXPECT_EQ(*++it, 2);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it, 3);

    // Test prefix and postfix decrement
    EXPECT_EQ(*--it, 2);
    EXPECT_EQ(*it--, 2);
    EXPECT_EQ(*it, 1);

    // Test equality
    auto it1 = v.begin();
    auto it2 = v.begin();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    ++it1;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);
}

TEST_F(VectorTest, ReverseIteratorOperators) {
    Vector<int> v{1, 2, 3};

    // Test prefix and postfix increment
    auto it = v.rbegin();
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(*++it, 2);
    EXPECT_EQ(*it++, 2);
    EXPECT_EQ(*it, 1);

    // Test prefix and postfix decrement
    EXPECT_EQ(*--it, 2);
    EXPECT_EQ(*it--, 2);
    EXPECT_EQ(*it, 3);

    // Test equality
    auto it1 = v.rbegin();
    auto it2 = v.rbegin();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    ++it1;
    EXPECT_FALSE(it1 == it2);
    EXPECT_TRUE(it1 != it2);
}

TEST_F(VectorTest, EdgeCases) {
    // Empty vector operations
    Vector<int> empty;
    EXPECT_THROW(empty.pop_back(), std::out_of_range);
    EXPECT_THROW(empty.front(), std::out_of_range);
    EXPECT_THROW(empty.back(), std::out_of_range);
}

TEST_F(VectorTest, EdgeCases1) {
    // Single element vector
    Vector<int> single{42};
    EXPECT_EQ(single.front(), 42);
    EXPECT_EQ(single.back(), 42);
    single.pop_back();
    EXPECT_TRUE(single.empty());
}

TEST_F(VectorTest, EdgeCases2) {
    // Large vector
    Vector<int> large(10000ULL, 7);
    EXPECT_EQ(large.size(), 10000);
    for (auto val : large) {
        EXPECT_EQ(val, 7);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}