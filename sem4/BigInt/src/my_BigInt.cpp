#include "my_BigInt.h"

int main() {
    BigInt a("678902324");
    BigInt b("777777777");
    BigInt c("154567899");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << a + b << std::endl;
    // std::cout << a * b << std::endl;
    // std::cout << (a * b) << std::endl;
}

