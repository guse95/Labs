#include <gtest/gtest.h>
#include "my_unique_ptr.h"

namespace my_smart_ptr {

    struct TestDeleter {
        bool* deleted;
        explicit TestDeleter(bool* d) : deleted(d) {}
        void operator()(const int* ptr) const {
            *deleted = true;
            delete ptr;
        }
    };

    struct TestArrayDeleter {
        bool* deleted;
        explicit TestArrayDeleter(bool* d) : deleted(d) {}
        void operator()(const int* ptr) const {
            *deleted = true;
            delete[] ptr;
        }
    };

    TEST(UniquePtrTest, DefaultConstructor) {
        UniquePtr<int> ptr;
        EXPECT_EQ(ptr.get(), nullptr);
        EXPECT_FALSE(static_cast<bool>(ptr));
    }

    TEST(UniquePtrTest, NullptrConstructor) {
        UniquePtr<int> ptr(nullptr);
        EXPECT_EQ(ptr.get(), nullptr);
        EXPECT_FALSE(static_cast<bool>(ptr));
    }

    TEST(UniquePtrTest, PointerConstructor) {
        int* raw = new int(42);
        UniquePtr<int> ptr(raw);
        EXPECT_EQ(ptr.get(), raw);
        EXPECT_EQ(*ptr, 42);
    }

    TEST(UniquePtrTest, MoveConstructor) {
        UniquePtr<int> ptr1(new int(42));
        UniquePtr<int> ptr2(std::move(ptr1));

        EXPECT_EQ(ptr1.get(), nullptr);
        EXPECT_NE(ptr2.get(), nullptr);
        EXPECT_EQ(*ptr2, 42);
    }

    TEST(UniquePtrTest, MoveAssignment) {
        UniquePtr<int> ptr1(new int(42));
        UniquePtr<int> ptr2;
        ptr2 = std::move(ptr1);

        EXPECT_EQ(ptr1.get(), nullptr);
        EXPECT_NE(ptr2.get(), nullptr);
        EXPECT_EQ(*ptr2, 42);
    }

    TEST(UniquePtrTest, CustomDeleter) {
        bool deleted = false;
        {
            UniquePtr<int, TestDeleter> ptr(new int(42), TestDeleter(&deleted));
            EXPECT_FALSE(deleted);
        }
        EXPECT_TRUE(deleted);
    }

    TEST(UniquePtrTest, Release) {
        int* raw = new int(42);
        UniquePtr<int> ptr(raw);
        int* released = ptr.release();

        EXPECT_EQ(released, raw);
        EXPECT_EQ(ptr.get(), nullptr);
        delete released;
    }

    TEST(UniquePtrTest, Reset) {
        bool deleted = false;
        int* raw1 = new int(42);
        int* raw2 = new int(24);

        {
            UniquePtr<int, TestDeleter> ptr(raw1, TestDeleter(&deleted));
            ptr.reset(raw2);
            EXPECT_TRUE(deleted);
            deleted = false;
            EXPECT_EQ(*ptr, 24);
        }
        EXPECT_TRUE(deleted);
    }

    TEST(UniquePtrTest, Swap) {
        UniquePtr<int> ptr1(new int(42));
        UniquePtr<int> ptr2(new int(24));

        ptr1.swap(ptr2);

        EXPECT_EQ(*ptr1, 24);
        EXPECT_EQ(*ptr2, 42);
    }

    TEST(UniquePtrTest, DereferenceOperators) {
        struct TestStruct {
            int value;
            explicit TestStruct(const int v) : value(v) {}
        };

        UniquePtr<TestStruct> ptr(new TestStruct(42));

        EXPECT_EQ((*ptr).value, 42);
        EXPECT_EQ(ptr->value, 42);
    }

    TEST(UniquePtrTest, BoolConversion) {
        UniquePtr<int> ptr1;
        UniquePtr<int> ptr2(new int(42));

        EXPECT_FALSE(static_cast<bool>(ptr1));
        EXPECT_TRUE(static_cast<bool>(ptr2));
    }

    TEST(UniquePtrArrayTest, DefaultConstructor) {
        UniquePtr<int[]> ptr;
        EXPECT_EQ(ptr.get(), nullptr);
        EXPECT_FALSE(static_cast<bool>(ptr));
    }

    TEST(UniquePtrArrayTest, ArrayConstructor) {
        int* raw = new int[3]{1, 2, 3};
        UniquePtr<int[]> ptr(raw);
        EXPECT_EQ(ptr.get(), raw);
        EXPECT_EQ(ptr[0], 1);
        EXPECT_EQ(ptr[1], 2);
        EXPECT_EQ(ptr[2], 3);
    }

    TEST(UniquePtrArrayTest, ArrayMoveConstructor) {
        UniquePtr<int[]> ptr1(new int[3]{1, 2, 3});
        UniquePtr<int[]> ptr2(std::move(ptr1));

        EXPECT_EQ(ptr1.get(), nullptr);
        EXPECT_NE(ptr2.get(), nullptr);
        EXPECT_EQ(ptr2[0], 1);
    }

    TEST(UniquePtrArrayTest, ArrayMoveAssignment) {
        UniquePtr<int[]> ptr1(new int[3]{1, 2, 3});
        UniquePtr<int[]> ptr2;
        ptr2 = (std::move(ptr1));

        EXPECT_EQ(ptr1.get(), nullptr);
        EXPECT_NE(ptr2.get(), nullptr);
        EXPECT_EQ(*ptr2, 1);
        EXPECT_EQ(ptr2[2], 3);
    }

    TEST(UniquePtrArrayTest, ArrayCustomDeleter) {
        bool deleted = false;
        {
            UniquePtr<int[], TestArrayDeleter> ptr(new int[3], TestArrayDeleter(&deleted));
            EXPECT_FALSE(deleted);
        }
        EXPECT_TRUE(deleted);
    }

    TEST(UniquePtrArrayTest, ArrayReset) {
        bool deleted = false;
        int* raw1 = new int[3];
        int* raw2 = new int[3];

        {
            UniquePtr<int[], TestArrayDeleter> ptr(raw1, TestArrayDeleter(&deleted));
            ptr.reset(raw2);
            EXPECT_TRUE(deleted);
            deleted = false;
        }
        EXPECT_TRUE(deleted);
    }

    TEST(UniquePtrArrayTest, ArrayIndexing) {
        UniquePtr<int[]> ptr(new int[3]{1, 2, 3});
        EXPECT_EQ(ptr[0], 1);
        EXPECT_EQ(ptr[1], 2);
        EXPECT_EQ(ptr[2], 3);

        ptr[1] = 42;
        EXPECT_EQ(ptr[1], 42);
    }

}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}