#pragma once
#include <iostream>
#include <vector>

class BigInt {
public:
    BigInt();
    BigInt(long long value);
    BigInt(const std::string& str);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    ~BigInt();

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt operator+=(const BigInt& other);
    BigInt operator-=(const BigInt& other);
    BigInt operator*=(const BigInt& other);
    BigInt operator/=(const BigInt& other);
    BigInt operator++();
    BigInt operator--();

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt mod_exp(const BigInt& exp, const BigInt& mod) const;
    BigInt fft_multiply(const BigInt& a) const;
    BigInt karatsuba_multiply(const BigInt& a) const;
    BigInt newton_divide(const BigInt& a) const;


    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
private:
    std::vector<unsigned long long> digits;
    bool isNegative;
};
