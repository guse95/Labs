#include "my_list.h"

namespace my_container {
    template <class T, class Allocator = std::allocator<T>>
    class Deque : public List<T, Allocator> {
    protected:

        std::size_t len = 0;
        std::size_t cap = 0;
        T *deque_ = nullptr;
        std::size_t front = 0;
        std::size_t back = 0;

        template <class IterType>
        class DequeIter
        {
        protected:
            friend class Deque;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit DequeIter(NoConstIterT *iter) : iter(iter) {}
            DequeIter(const DequeIter &other) = default;
            DequeIter(DequeIter &&other) = default;

        public:
            DequeIter operator++() {
                ++this->iter;
                return *this;
            }
            DequeIter operator++(int) {
                DequeIter temp = *this;
                ++this->iter;
                return temp;
            }
            DequeIter operator--() {
                --this->iter;
                return *this;
            }
            DequeIter operator--(int) {
                DequeIter temp = *this;
                --this->iter;
                return temp;
            }
            bool operator==(const DequeIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const DequeIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

        template <class IterType>
        class DequeReverseIter
        {
        protected:
            friend class Array;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit DequeReverseIter(NoConstIterT *iter) : iter(iter) {}
            DequeReverseIter(const DequeReverseIter &other) = default;
            DequeReverseIter(DequeReverseIter &&other) = default;

        public:
            DequeReverseIter operator++() {
                --this->iter;
                return *this;
            }
            DequeReverseIter operator++(int) {
                DequeIter temp = *this;
                --this->iter;
                return temp;
            }
            DequeReverseIter operator--() {
                ++this->iter;
                return *this;
            }
            DequeReverseIter operator--(int) {
                DequeIter temp = *this;
                ++this->iter;
                return temp;
            }
            bool operator==(const DequeReverseIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const DequeReverseIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

    public:
        using iterator = DequeIter<T>;
        using ConstIterator = DequeIter<const T>;
        using ReverseIterator = DequeReverseIter<T>;
        using ConstReverseIterator = DequeReverseIter<const T>;
    };
}