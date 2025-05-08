#include "my_BigInt.h"

int main() {
    BigInt a("7647589684286572184845726784719278628342164034575675777");
    BigInt b("478263746286482647826374875891820482738468240284");
    BigInt c("154567899");
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << a.fft_multiply(b) << std::endl;
    std::cout << a * b << std::endl;
}