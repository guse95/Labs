#include <iostream>

using namespace std;

double comb(double m, double n) {
    double res = 1;
    for (double i = m + 1; i <= n; i += 1) {
        res *= i;
    }
    for (double i = 2; i <= (n - m); i += 1) {
        res /= i;
    }
    return res;
}

int main(){
    std::cout << "HELLO WORLD\n";
    char f[] = {'0','.','2','1' };
    int n = 2;
    double m = comb(2, 5);
    printf("%f", m);
}
