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
            ArrayIter operator++() {
                ++this->iter;
                return *this;
            }
            ArrayIter operator++(int) {
                ArrayIter temp = *this;
                ++this->iter;
                return temp;
            }
            ArrayIter operator--() {
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
            ArrayReverseIter operator++() {
                --this->iter;
                return *this;
            }
            ArrayReverseIter operator++(int) {
                ArrayIter temp = *this;
                --this->iter;
                return temp;
            }
            ArrayReverseIter operator--() {
                ++this->iter;
                return *this;
            }
            ArrayReverseIter operator--(int) {
                ArrayIter temp = *this;
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

        Array() : cap(N), arr(new T[this->cap]) {}
        explicit Array(const std::size_t len, const T& val) : cap(N) {
            arr = new T[cap];
            for (std::size_t i = 0; i < len && i < cap; i++) {
                arr[i] = val;
            }
        }
        Array(const Array &other) : cap(other.cap) {
            arr = new T[cap];
            std::copy(other.begin(), other.end(), arr);
        };
        Array(Array &&other)  noexcept : cap(other.cap), arr(other.arr) {
            other.arr = nullptr;
            other.cap = 0;
        }
        Array(std::initializer_list<T> init) : cap(N) {
            arr = new T[cap];
            std::size_t i = 0;
            for (const auto el : init) {
                arr[i++] = el;
                if (i >= cap) break;
            }
        }
        Array(std::initializer_list<const std::pair<size_t, T>> init) : cap(N) {
            arr = new T[cap];
            for (const auto [ind, val] : init) {
                if (ind < N) {
                    arr[ind] = val;
                }
            }
        }

        ~Array() final{
            delete[] this->arr;
        }

        Array& operator=(const Array &other) {
            if (other and this != &other) {
                delete[] this->arr;
                this->cap = other.cap;
                this->arr = new T[this->cap];
                std::copy(other.begin(), other.end(), this->arr);
            }
            return *this;
        }

        T& at(std::size_t poz) {
            if (poz >= this->cap) {
                throw std::out_of_range("");
            }
            return this->arr[poz];
        }
        const T& at(std::size_t poz) const{
            if (poz >= this->cap) {
                throw std::out_of_range("");
            }
            return this->arr[poz];
        }

        T& operator[](std::size_t poz) {
            return this->arr[poz];
        }
        const T& operator[](std::size_t poz) const {
            return this->arr[poz];
        }

        T& front() {
            return this->arr[0];
        }
        const T& front() const {
            return this->arr[0];
        }

        T& back() {
            return this->arr[this->cap - 1];
        }
        const T& back() const {
            return this->arr[this->cap - 1];
        }

        T* data() noexcept {
            return this->arr;
        }
        const T* data() const noexcept {
            return this->arr;
        }

        iterator begin() noexcept {
            return this->arr;
        }
        iterator end() noexcept {
            return this->arr + this->cap;
        }
        ConstIterator cbegin() const noexcept {
            return this->arr;
        }
        ConstIterator cend() const noexcept {
            return this->arr + this->cap;
        }
        ReverseIterator rbegin() noexcept {
            return this->arr + this->cap - 1;
        }
        ReverseIterator rend() noexcept {
            return this->arr - 1;
        }
        ConstReverseIterator crbegin() const noexcept {
            return this->arr + this->cap - 1;
        }
        ConstReverseIterator crend() const noexcept {
            return this->arr - 1;
        }

        [[nodiscard]] bool empty() const noexcept final {
            return this->cap == 0;
        }

        [[nodiscard]] std::size_t size() const noexcept final {
            return this->cap;
        }

        [[nodiscard]] std::size_t max_size() const noexcept final {
            return this->cap;
        }

        void fill(const T& val) {
            std::fill(this->arr, this->arr + this->cap, val);
        }

        void swap(Array &other) noexcept {
            std::swap(this->arr, other.arr);
            std::swap(this->cap, other.cap);
        }

        bool operator==(const Array &other) const {
            if (this->cap != other.cap) {
                return false;
            }
            for (std::size_t i = 0; i < this->cap; ++i) {
                if (this->arr[i] != other.arr[i]) {
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

        std::weak_ordering operator<=>(const Array &other) const {
            if (*this == other) {
                return std::weak_ordering::equivalent;
            }
            if (*this < other) {
                return std::weak_ordering::less;
            }
            return std::weak_ordering::greater;
        }


    };
}