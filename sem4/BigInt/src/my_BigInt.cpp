#include "my_BigInt.h"

int main() {
    BigInt a("2579");
    BigInt b("371");
    BigInt c("65537");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << a.newton_divide(b) << std::endl;
    std::cout << a / b << std::endl;
}