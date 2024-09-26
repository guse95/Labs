#include <iostream>

using namespace std;

int main(){
    std::cout << "HELLO WORLD\n";
    char f[] = {'0','.','2','1' };
    float m = strtod(f, NULL);
    printf("%f", m);
}
