#include "my_deque.h"


int main() {
    my_container::Deque<const int> my_deque{2, 3, 4, 5};
    my_deque.push_back(52);
    my_deque.push_back(53);
    std::cout << my_deque.back() << std::endl;
}