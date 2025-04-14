#include "my_array.h"
#include "my_list.h"


int main() {
    // constexpr std::allocator<int> all;
    // my_container::List<int, std::allocator<int>> my_list(2, 5, all);
    // my_list.insert(my_list.cbegin(), 52);
    // for (const int i : my_list) {
    //     std::cout << i << std::endl;
    // }

    my_container::Array<int, 4> my_array(5);
}
