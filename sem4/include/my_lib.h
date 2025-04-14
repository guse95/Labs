#pragma once
#include "../src/my_container.h"

namespace my_container {
    template <class T, std::size_t N>
    class Array : public Container<T>
    {
    protected:
        std::size_t cap = 0;
        T *arr = nullptr;

        template <class IterType>
        class ArrayIter
        {
        protected:
            friend class Array;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit ArrayIter(NoConstIterT *iter) : iter(iter) {}
            ArrayIter(const ArrayIter &other) = default;
            ArrayIter(ArrayIter &&other) = default;

        public:
            ArrayIter& operator++() {
                ++this->iter;
                return *this;
            }
            ArrayIter operator++(int) {
                ArrayIter temp = *this;
                ++this->iter;
                return temp;
            }
            ArrayIter& operator--() {
                --this->iter;
                return *this;
            }
            ArrayIter operator--(int) {
                ArrayIter temp = *this;
                --this->iter;
                return temp;
            }
            bool operator==(const ArrayIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const ArrayIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

        template <class IterType>
        class ArrayReverseIter
        {
        protected:
            friend class Array;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit ArrayReverseIter(NoConstIterT *iter) : iter(iter) {}
            ArrayReverseIter(const ArrayReverseIter &other) = default;
            ArrayReverseIter(ArrayReverseIter &&other) = default;

        public:
            ArrayReverseIter& operator++() {
                --this->iter;
                return *this;
            }
            ArrayReverseIter operator++(int) {
                ArrayReverseIter temp = *this;
                --this->iter;
                return temp;
            }
            ArrayReverseIter& operator--() {
                ++this->iter;
                return *this;
            }
            ArrayReverseIter operator--(int) {
                ArrayReverseIter temp = *this;
                ++this->iter;
                return temp;
            }
            bool operator==(const ArrayReverseIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const ArrayReverseIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

    public:
        using iterator = ArrayIter<T>;
        using ConstIterator = ArrayIter<const T>;
        using ReverseIterator = ArrayReverseIter<T>;
        using ConstReverseIterator = ArrayReverseIter<const T>;

        Array() : cap(N), arr(new T[cap]) {}
        explicit Array(const std::size_t len, const T& val) :
        cap(N), arr(new T[len]) {
            for (std::size_t i = 0; i < len && i < cap; i++) {
                arr[i] = val;
            }
        }
        Array(const Array &other) :
        cap(other.cap), arr(new T[cap]) {
            std::copy(other.arr, other.arr + cap, arr);
        };
        explicit Array(Array &&other)  noexcept :
        cap(other.cap), arr(other.arr) {
            other.arr = nullptr;
            other.cap = 0;
        }
        explicit Array(std::initializer_list<T> init) :
        cap(N), arr(new T[cap]) {
            std::size_t i = 0;
            for (const auto el : init) {
                if (i >= cap) break;
                arr[i++] = el;
            }
        }
        explicit Array(std::initializer_list<const std::pair<size_t, T>> init) :
        cap(N), arr(new T[cap]) {
            for (const auto& [ind, val] : init) {
                if (ind < cap) {
                    arr[ind] = val;
                }
            }
        }

        ~Array() final{
            delete[] arr;
        }

        Array& operator=(const Array &other) {
            if (this != &other) {
                delete[] arr;
                cap = other.cap;
                arr = new T[cap];
                std::copy(other.arr, other.arr + cap, this->arr);
            }
            return *this;
        }

        Array& operator=(Array &&other) noexcept {
            if (this != &other) {
                delete[] arr;
                cap = other.cap;
                arr = other.arr;
                other.arr = nullptr;
                other.cap = 0;
            }
            return *this;
        }

        T& at(std::size_t poz) {
            if (poz >= cap) {
                throw std::out_of_range("Index out of range");
            }
            return arr[poz];
        }
        const T& at(std::size_t poz) const{
            if (poz >= cap) {
                throw std::out_of_range("Index out of range");
            }
            return arr[poz];
        }

        T& operator[](std::size_t poz) {
            return arr[poz];
        }
        const T& operator[](std::size_t poz) const {
            return arr[poz];
        }

        T& front() {
            return arr[0];
        }
        const T& front() const {
            return arr[0];
        }

        T& back() {
            return arr[cap - 1];
        }
        const T& back() const {
            return arr[cap - 1];
        }

        T* data() noexcept {
            return arr;
        }
        const T* data() const noexcept {
            return arr;
        }

        iterator begin() noexcept {
            return iterator(arr);
        }
        iterator end() noexcept {
            return iterator(arr + cap);
        }
        ConstIterator cbegin() const noexcept {
            return ConstIterator(arr);
        }
        ConstIterator cend() const noexcept {
            return ConstIterator(arr + cap);
        }
        ReverseIterator rbegin() noexcept {
            return ReverseIterator(arr + cap - 1);
        }
        ReverseIterator rend() noexcept {
            return ReverseIterator(arr - 1);
        }
        ConstReverseIterator crbegin() const noexcept {
            return ConstReverseIterator(arr + cap - 1);
        }
        ConstReverseIterator crend() const noexcept {
            return ConstReverseIterator(arr - 1);
        }

        [[nodiscard]] bool empty() const noexcept final {
            return cap == 0;
        }

        [[nodiscard]] std::size_t size() const noexcept final {
            return cap;
        }

        [[nodiscard]] std::size_t max_size() const noexcept final {
            return cap;
        }

        void fill(const T& val) {
            std::fill(arr, arr + cap, val);
        }

        void swap(Array &other) noexcept {
            std::swap(arr, other.arr);
            std::swap(cap, other.cap);
        }

        bool operator==(const Array &other) const {
            if (cap != other.cap) {
                return false;
            }
            for (std::size_t i = 0; i < cap; ++i) {
                if (arr[i] != other.arr[i]) {
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const Array &other) const {
            return !(*this == other);
        }
        bool operator<(const Array &other) const {
            const std::size_t less_len = (this->cap > other.cap) ? other.cap : this->cap;

            for (std::size_t i = 0; i < less_len; ++i) {
                if (this->arr[i] < other.arr[i]) {
                    return true;
                }
                if (this->arr[i] > other.arr[i]) {
                    return false;
                }
            }
            return this->cap < other.cap;
        }
        bool operator<=(const Array &other) const {
            return (*this < other || *this == other);
        }
        bool operator>(const Array &other) const {
            return !(*this <= other);
        }
        bool operator>=(const Array &other) const {
            return !(*this < other);
        }
#if __cplusplus >= 202002L
        constexpr auto operator<=>(const Array &other) const {
            if (*this == other) {
                return std::weak_ordering::equivalent;
            }
            if (*this < other) {
                return std::weak_ordering::less;
            }
            return std::weak_ordering::greater;
        }
#endif
    };
}