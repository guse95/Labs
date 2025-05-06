#include "my_BigInt.h"

int main() {
    BigInt a("-764758968428034575675777");
    BigInt b("478263746286482647826374");
    BigInt c("154567899");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << a.karatsuba_multiply(b) << std::endl;
    std::cout << a * b << std::endl;
}