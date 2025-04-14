#include "my_lib.h"


int main() {
    // constexpr std::allocator<int> all;
    // my_container::List<int, std::allocator<int>> my_list(2, 5, all);
    // my_list.insert(my_list.cbegin(), 52);
    // for (const int i : my_list) {
    //     std::cout << i << std::endl;
    // }

    my_container::Array<int, 6> my_array {2, 3, 4, 7, 6, 8};
    std::cout << my_array.front() << std::endl;
    std::cout << my_array.back() << std::endl;
    std::cout << my_array.at(4) << std::endl;

    for (int i = 0; i < 6; i++) {
        std::cout << my_array[i] << std::endl;
    }
}