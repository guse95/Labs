#include "my_list.h"


int main() {

    my_container::List<int> my_list;
    my_list.push_back(52);
    my_list.push_back(3);
    for (const int i : my_list) {
        std::cout << i << std::endl;
    }
    std::cout << my_list.empty() << std::endl;
    std::cout << (my_list.size() == 2) << std::endl;
}
//SFINAE