#pragma once
#include <memory>

namespace my_smart_ptr {
        template <class T, class Deleter = std::default_delete<T>>
        class UniquePtr {

        };
}
