#include "my_lib.h"


int main() {
    my_container::Array<int, 6> my_array {2, 3, 4, 7, 6, 8};
    std::cout << my_array.front() << std::endl;
    std::cout << my_array.back() << std::endl;
    std::cout << my_array.at(4) << std::endl;

    for (int i = 0; i < 6; i++) {
        std::cout << my_array[i] << std::endl;
    }
}