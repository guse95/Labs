#pragma once
#include "../../Deque/include/my_deque.h"


namespace my_container {
    template <class T, class Container = Deque<T>>
    class Stack {
        Container st;

        public:
        Stack() :
        st(Container()) {}

        explicit Stack(const Container &d) :
        st(Container(d)) {}

        explicit Stack(Container &&d) :
        st(Container(std::move(d))) {}

        Stack(const Stack &other) :
        Stack(other.st) {}

        Stack(Stack &&other) noexcept :
        Stack(std::move(other.st)) {}

        template<class InputIt>
        Stack(InputIt first, InputIt last) :
        st(Container(first, last)) {}

        template<class Allocator>
        explicit Stack(const Allocator& alloc) :
        st(Container(alloc)) {}

        template<class Allocator>
        explicit Stack(const Container &other, const Allocator& alloc) :
        st(Container(other, alloc)) {}

        template<class Allocator>
        Stack(Container &&other, const Allocator& alloc) :
        st(Container(std::move(other), alloc)) {}

        template<class Allocator>
        Stack(const Stack &other, const Allocator& alloc) :
        Stack(other.st, alloc) {}

        template<class Allocator>
        Stack(Stack &&other, const Allocator& alloc) :
        Stack(std::move(other.st), alloc) {}

        template<class InputIt, class Allocator>
        Stack( InputIt first, InputIt last, const Allocator& alloc) :
        st(Container(first, last, alloc)) {}

        Stack(std::initializer_list<T> init) :
        Stack(init.begin(), init.end()) {}

        template<class Allocator>
        Stack(std::initializer_list<T> init, Allocator alloc) :
        Stack(init.begin(), init.end(), alloc) {}

        ~Stack() = default;

        Stack &operator=(const Stack &other) {
            st = other.st;
            return *this;
        }
        Stack &operator=(Stack &&other) noexcept {
            st = std::move(other.st);
            return *this;
        }

        T& top() {
            return st.back();
        }
        const T& top() const {
            return st.back();
        }

        bool empty() const {
            return st.empty();
        }
        size_t size() const {
            return st.size();
        }

        void push(const T& val) {
            st.push_back(val);
        }
        void push(T&& val) {
            st.push_back(std::move(val));
        }
        void pop() {
            st.pop_back();
        }

        void swap(Stack &other) noexcept {
            st.swap(other.st);
        }

        bool operator==(const Stack &other) const {
            return st == other.st;
        }
        bool operator!=(const Stack &other) const {
            return !(*this == other);
        }
        bool operator<(const Stack &other) const {
            return st < other.st;
        }
        bool operator>(const Stack &other) const {
            return st > other.st;
        }
        bool operator<=(const Stack &other) const {
            return st <= other.st;
        }
        bool operator>=(const Stack &other) const {
            return st >= other.st;
        }
#if __cplusplus >= 202002L
        int operator<=>( const Stack& other ) const {
            return st <=> other.st;
        }
#endif
    };
}