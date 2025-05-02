#include "my_BigInt.h"

int main() {
    BigInt a("123456789");
    BigInt b("1");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << (a / b) << std::endl;
    // std::cout << (a - b) << std::endl;
    // std::cout << (a * b) << std::endl;
}