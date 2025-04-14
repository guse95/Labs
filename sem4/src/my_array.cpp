// #include "my_array.h"

#include "my_list.h"


int main() {
    constexpr std::allocator<int> all;
    my_container::List<int, std::allocator<int>> my_list(2, 5, all);
    my_list.push_front(1);
    my_list.push_front(2);
    my_list.push_back(3);
    my_list.push_back(4);
    for (const auto it : my_list) {
        std::cout << it << std::endl;
    }
}
