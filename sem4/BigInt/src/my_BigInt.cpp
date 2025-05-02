#include "my_BigInt.h"

int main() {
    BigInt a("3456789029853123812389618369183");
    BigInt b("15231623791273513123");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << (a / b) << std::endl;
    // std::cout << (a - b) << std::endl;
    // std::cout << (a * b) << std::endl;
}