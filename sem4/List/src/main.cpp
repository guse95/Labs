#include <vector>

#include "my_list.h"


int main() {

    my_container::List<const int> my_list{1, 2, 3};
    my_container::List<const int> goat{1, 2, 3};

    for (const int i : my_list) {
        std::cout << i << std::endl;
    }

    // std::vector<int> vec{1, 2, 3};
    // my_container::List<int> list(vec.begin(), vec.end());
    //
    // for (const int i : list) {
    //     std::cout << i << std::endl;
    // }
    std::cout << (goat <=> my_list) << std::endl;
}
//SFINAE