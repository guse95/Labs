#include <iostream>
#include <cmath>
int main() {
    double x = 1.5;
    int n = 3;
    double result = std::scalbln(x, n);
    std::cout << "Масштабированное значение: " << result << std::endl;
    return 0;
}