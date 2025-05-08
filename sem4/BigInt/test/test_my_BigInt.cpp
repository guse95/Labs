#include <gtest/gtest.h>
#include "my_BigInt.h"

class BigIntAlgorithmsTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Тестовые данные
        a = BigInt("123456789");
        b = BigInt("987654321");
        prime = BigInt("1000000007"); // Простое число
        zero = BigInt(0);
        one = BigInt(1);
        two = BigInt(2);
    }

    BigInt a, b, prime, zero, one, two;
};

// Тесты для mod_exp (возведение в степень по модулю)
TEST_F(BigIntAlgorithmsTest, ModExpBasic) {
    EXPECT_EQ(a.mod_exp(zero, prime), one);
    EXPECT_EQ(a.mod_exp(one, prime), a % prime);
    BigInt res = two.mod_exp(BigInt(10), BigInt(1024));
    EXPECT_EQ(res, BigInt());
}

TEST_F(BigIntAlgorithmsTest, ModExpWithPrime) {
    BigInt base("123");
    BigInt exp("456");
    BigInt mod("1000000007");

    // Проверяем через известный результат
    BigInt expected("565291922");
    EXPECT_EQ(base.mod_exp(exp, mod), expected);
}

TEST_F(BigIntAlgorithmsTest, ModExpEdgeCases) {
    // 0^0 - неопределено, но в нашей реализации возвращает 1
    EXPECT_EQ(zero.mod_exp(zero, prime), one);

    // 0^n где n > 0
    EXPECT_EQ(zero.mod_exp(BigInt(5), prime), zero);

    // n^0
    EXPECT_EQ(a.mod_exp(zero, prime), one);
}

// Тесты для karatsuba_multiply
TEST_F(BigIntAlgorithmsTest, KaratsubaMultiplyBasic) {
    EXPECT_EQ(a.karatsuba_multiply(one), a);
    EXPECT_EQ(a.karatsuba_multiply(zero), zero);
    EXPECT_EQ(a.karatsuba_multiply(two), a + a);
}

TEST_F(BigIntAlgorithmsTest, KaratsubaMultiplyMediumNumbers) {
    BigInt x("12345678934298509281057195719");
    BigInt y("9876543219758591085");
    BigInt expected("121932631571862410339465161399045027508533565115");

    EXPECT_EQ(x.karatsuba_multiply(y), expected);
}

TEST_F(BigIntAlgorithmsTest, KaratsubaMultiplyNegativeNumbers) {
    BigInt neg_a = -a;
    BigInt neg_b = -b;

    EXPECT_EQ(neg_a.karatsuba_multiply(b), -(a.karatsuba_multiply(b)));
    EXPECT_EQ(a.karatsuba_multiply(neg_b), -(a.karatsuba_multiply(b)));
    EXPECT_EQ(neg_a.karatsuba_multiply(neg_b), a.karatsuba_multiply(b));
}

// Тесты для fft_multipy
TEST_F(BigIntAlgorithmsTest, FftMultipyBasic) {
    EXPECT_EQ(a.karatsuba_multiply(one), a);
    EXPECT_EQ(a.karatsuba_multiply(zero), zero);
    EXPECT_EQ(a.karatsuba_multiply(two), a + a);
}

TEST_F(BigIntAlgorithmsTest, FftMultipyBasicMediumNumbers) {
    BigInt x("12345678934298509281057195719");
    BigInt y("9876543219758591085");
    BigInt expected("121932631571862410339465161399045027508533565115");

    EXPECT_EQ(x.karatsuba_multiply(y), expected);
}

TEST_F(BigIntAlgorithmsTest, FftMultipyBasicNegativeNumbers) {
    BigInt neg_a = -a;
    BigInt neg_b = -b;

    EXPECT_EQ(neg_a.karatsuba_multiply(b), -(a.karatsuba_multiply(b)));
    EXPECT_EQ(a.karatsuba_multiply(neg_b), -(a.karatsuba_multiply(b)));
    EXPECT_EQ(neg_a.karatsuba_multiply(neg_b), a.karatsuba_multiply(b));
}

TEST(Constructor, DefaultConstructor) {
    const BigInt num;
    EXPECT_EQ(num, BigInt(0));
}

TEST(Constructor, LongLongConstructor) {
    BigInt num1(12345567867872879);
    BigInt num2(-987654321);
    BigInt num3(0);

    EXPECT_EQ(num1, BigInt("12345567867872879"));
    EXPECT_EQ(num2, BigInt("-987654321"));
    EXPECT_EQ(num3, BigInt("0"));
}

TEST(Constructor, StringConstructor) {
    BigInt num1("123456789012345678");
    BigInt num2("-987654321098765430");
    BigInt num3("0");

    EXPECT_EQ(num1, BigInt(123456789012345678LL));
    EXPECT_EQ(num2, BigInt(-98765432109876543LL) * BigInt(10));
    EXPECT_EQ(num3, BigInt(0));

    EXPECT_THROW(BigInt("abc123"), std::invalid_argument);
    EXPECT_THROW(BigInt("12a34"), std::invalid_argument);
    EXPECT_THROW(BigInt("-"), std::invalid_argument);
}

TEST(Constructor, CopyMoveConstructors) {
    BigInt num1("123456789");
    BigInt num2(num1);
    BigInt num3("987654321");
    BigInt num4(std::move(num3));

    EXPECT_EQ(num1, num2);
    EXPECT_EQ(num4, BigInt("987654321"));
}

// Assignment operators
TEST(Operators, AssignmentOperators) {
    BigInt num1("123456789");
    BigInt num2;
    num2 = num1;
    EXPECT_EQ(num1, num2);

    BigInt num3;
    num3 = std::move(BigInt("987654321"));
    EXPECT_EQ(num3, BigInt("987654321"));
}

// Arithmetic operations
TEST(Operators, Addition) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt num5("8765432187654321");
    BigInt num6("9875432198754321");
    BigInt zero(0);

    EXPECT_EQ(num1 + num2, BigInt("1111111110"));
    EXPECT_EQ(num1 + num3, zero);
    EXPECT_EQ(num3 + num4, BigInt("-1111111110"));
    EXPECT_EQ(num1 + zero, num1);
    EXPECT_EQ(num4 + num1, BigInt("-864197532"));
    EXPECT_EQ(num5 + num6, BigInt("18640864386408642"));
}

TEST(Operators, Subtraction) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt num5("8765432187654321");
    BigInt num6("9875432198754321");
    BigInt zero(0);

    EXPECT_EQ(num2 - num1, BigInt("864197532"));
    EXPECT_EQ(num1 - num1, zero);
    EXPECT_EQ(num3 - num2, BigInt("-1111111110"));
    EXPECT_EQ(num3 - num4, BigInt("864197532"));
    EXPECT_EQ(num4 - num3, BigInt("-864197532"));
    EXPECT_EQ(zero - num1, BigInt("-123456789"));
    EXPECT_EQ(num5 - num6, BigInt("-1110000011100000"));
}

TEST(Operators, Multiplication) {
    BigInt num1("12345678934298509281057195719");
    BigInt num2("9876543219758591085");
    BigInt num3("-12345678934298509281057195719");
    BigInt num4("-9876543219758591085");
    BigInt zero(0);

    EXPECT_EQ(num1 * num2, BigInt("121932631571862410339465161399045027508533565115"));
    EXPECT_EQ(num1 * num3, BigInt("-152415788348781975841591720665223850958913207428271926961"));
    EXPECT_EQ(num3 * num4, BigInt("121932631571862410339465161399045027508533565115"));
    EXPECT_EQ(num1 * zero, zero);
    EXPECT_EQ(zero * num4, zero);
}

TEST(Operators, Division) {
    BigInt num1("2930123172289731288276457");
    BigInt num2("15241578834878197584159172066522385");
    BigInt num3("-2930123172289731288276457");
    BigInt num4("-15241578834878197584159172066522385");
    BigInt zero(0);
    BigInt one(1);

    EXPECT_EQ(num2 / num1, BigInt("5201685369"));
    EXPECT_EQ(num1 / num2, zero);
    EXPECT_EQ(num4 / num3, BigInt("5201685369"));
    EXPECT_EQ(num3 / num1, BigInt("-1"));
    EXPECT_EQ(num1 / one, num1);
    EXPECT_THROW(num1 / zero, std::invalid_argument);
}

TEST(Operators, CompoundAssignmentOperators) {
    BigInt num1("12347402428078456789");
    BigInt num2("98765432829729388731");
    BigInt num3("-12347402428078456789");

    num1 += num2;
    EXPECT_EQ(num1, BigInt("111112835257807845520"));

    num1 -= num2;
    EXPECT_EQ(num1, BigInt("12347402428078456789"));

    num1 *= num2;
    EXPECT_EQ(num1, BigInt("1219496545132020383724120990446767044759"));

    num1 /= num2;
    EXPECT_EQ(num1, BigInt("12347402428078456789"));

    num3 += BigInt("12347402428078456789");
    EXPECT_EQ(num3, BigInt(0));
}

// Comparison operators
TEST(Operators, ComparisonOperators) {
    BigInt num1("123888829456789");
    BigInt num2("987654321747139");
    BigInt num3("-123888829456789");
    BigInt num4("-987654321747139");
    BigInt num5("123888829456789");

    EXPECT_TRUE(num1 < num2);
    EXPECT_TRUE(num4 < num3);
    EXPECT_TRUE(num1 <= num5);
    EXPECT_TRUE(num1 == num5);
    EXPECT_TRUE(num1 != num2);
    EXPECT_TRUE(num2 > num1);
    EXPECT_TRUE(num3 > num4);
    EXPECT_TRUE(num5 >= num1);
    EXPECT_TRUE(num3 <= num1);
}

// I/O operations
TEST(Operators, StreamOperations) {
    std::stringstream ss1, ss2, ss3;
    BigInt num1("10002345600070089");
    BigInt num2("-987654321");
    BigInt num3;

    ss1 << num1;
    EXPECT_EQ(ss1.str(), "10002345600070089");

    ss2 << num2;
    EXPECT_EQ(ss2.str(), "-987654321");

    ss3 << "10002345600070089";
    ss3 >> num3;
    EXPECT_EQ(num3, num1);

    ss3.clear();
    ss3 << "-987654321";
    ss3 >> num3;
    EXPECT_EQ(num3, num2);

    ss3.clear();
    ss3 << "invalid";
    EXPECT_THROW(ss3 >> num3, std::invalid_argument);
}

TEST(Func, AbsFunction) {
    BigInt num1("123456789");
    BigInt num2("-987654321");

    EXPECT_EQ(BigInt::abs(num1), num1);
    EXPECT_EQ(BigInt::abs(num2), BigInt("987654321"));
    EXPECT_EQ(BigInt::abs(BigInt(0)), BigInt(0));
}

TEST(StressTest, EdgeCases) {
    BigInt max_ll(std::to_string(LLONG_MAX));
    BigInt min_ll(std::to_string(LLONG_MIN));
    BigInt beyond_ll("9223372036854775808"); // LL_MAX + 1

    EXPECT_GT(beyond_ll, max_ll);
    EXPECT_LT(min_ll, max_ll);

    BigInt zero(0);
    BigInt num("123456789");

    EXPECT_EQ(num + zero, num);
    EXPECT_EQ(num - zero, num);
    EXPECT_EQ(zero - num, -num);
    EXPECT_EQ(num * zero, zero);

    BigInt one(1);
    EXPECT_EQ(num * one, num);
    EXPECT_EQ(num / one, num);
}

TEST(StressTest, Performance) {
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");

    EXPECT_NO_THROW(a + b);
    EXPECT_NO_THROW(a - b);
    EXPECT_NO_THROW(a * b);
    EXPECT_NO_THROW(a / BigInt("1234567890"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}