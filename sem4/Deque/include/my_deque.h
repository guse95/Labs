#pragma once
#include "../../List/include/my_list.h"

namespace my_container {
    template <class T, class Allocator = std::allocator<T>>
    class Deque : public List<T, Allocator> {
    public:
        using Base = List<T, Allocator>;
        using node = typename Base::node;

        using Base::Head;
        using Base::Tail;
        using Base::len;

        Deque() : Base() {}

        explicit Deque(const std::size_t cnt, const Allocator& alloc_ = Allocator()) :
            Base(cnt, alloc_) {}

        Deque(const std::size_t cnt, const T& val_, const Allocator& alloc_ = Allocator()) :
            Base(cnt, val_, alloc_) {}

        template<class InputIt>
        Deque(InputIt first, InputIt last, const Allocator& alloc_ = Allocator()) :
            Base(first, last, alloc_) {}

        Deque(const std::initializer_list<T>& initList) : Base(initList) {}

        Deque(const Deque& other) : Base(other) {}

        Deque(Deque&& other) noexcept : Base(std::move(other)) {}

        Deque& operator=(const Deque& other) {
            Base::operator=(other);
            return *this;
        }

        Deque& operator=(Deque&& other) noexcept {
            Base::operator=(std::move(other));
            return *this;
        }

        ~Deque() override = default;

        T& operator[](std::size_t index) {
            if (index >= len) {
                throw std::out_of_range("Deque index out of range");
            }
            node* current = Head;
            for (std::size_t i = 0; i < index; ++i) {
                current = current->next;
            }
            return current->val;
        }

        const T& operator[](std::size_t index) const {
            return operator[](index);
        }

        T& at(std::size_t index) {
            return operator[](index);
        }

        const T& at(std::size_t index) const {
            return operator[](index);
        }
    };
}