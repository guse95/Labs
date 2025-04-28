#pragma once
#include <limits>
#include "../../Lab1/src/my_container.h"

namespace my_container {
    template<class T, class Allocator = std::allocator<T>>
    class Vector : public Container<T> {
    protected:
        Allocator alloc;
        T* vec = nullptr;
        std::size_t sz = 0, cap = 0;

        template <class IterType>
        class VectorIter
        {
        protected:
            friend class Vector;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
        public:
            explicit VectorIter(NoConstIterT *iter) : iter(iter) {}
            VectorIter(const VectorIter &other) = default;
            VectorIter(VectorIter &&other) = default;

            VectorIter& operator=(const VectorIter& other) = default;
            VectorIter& operator=(VectorIter&& other) = default;

            VectorIter& operator++() {
                ++this->iter;
                return *this;
            }
            VectorIter operator++(int) {
                VectorIter temp = *this;
                ++this->iter;
                return temp;
            }
            VectorIter operator+(const std::size_t cnt) {
                T* tmp = iter + cnt;
                return VectorIter(tmp);
            }
            VectorIter& operator--() {
                --this->iter;
                return *this;
            }
            VectorIter operator--(int) {
                VectorIter temp = *this;
                --this->iter;
                return temp;
            }
            VectorIter operator-(const std::size_t cnt) {
                return VectorIter(iter - cnt);
            }
            bool operator==(const VectorIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const VectorIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

        template <class IterType>
        class VectorReverseIter
        {
        protected:
            friend class Vector;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
        public:
            explicit VectorReverseIter(NoConstIterT *iter) : iter(iter) {}
            VectorReverseIter(const VectorReverseIter &other) = default;
            VectorReverseIter(VectorReverseIter &&other) = default;

            VectorReverseIter& operator=(const VectorReverseIter& other) = default;
            VectorReverseIter& operator=(VectorReverseIter &&other) = default;

            VectorReverseIter& operator++() {
                --this->iter;
                return *this;
            }
            VectorReverseIter operator++(int) {
                VectorReverseIter temp = *this;
                --this->iter;
                return temp;
            }
            VectorReverseIter operator+(const std::size_t cnt) {
                return VectorIter(iter - cnt);
            }
            VectorReverseIter& operator--() {
                ++this->iter;
                return *this;
            }
            VectorReverseIter operator--(int) {
                VectorReverseIter temp = *this;
                ++this->iter;
                return temp;
            }
            VectorReverseIter operator-(const std::size_t cnt) {
                return VectorIter(iter + cnt);
            }
            bool operator==(const VectorReverseIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const VectorReverseIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter;
            }
        };

        void clear_all() {
            if (vec != nullptr) {
                alloc.deallocate(vec, cap);
                cap = 0;
                sz = 0;
            }
        }

        template <class InputIt>
        std::size_t distance(InputIt first, InputIt last) noexcept {
            std::size_t dist = 0;
            while (first != last) {
                ++first;
                ++dist;
            }
            return dist;
        }
        
    public:
        using iterator = VectorIter<T>;
        using ConstIterator = VectorIter<const T>;
        using ReverseIterator = VectorReverseIter<T>;
        using ConstReverseIterator = VectorReverseIter<const T>;

        Vector() noexcept(noexcept(Allocator())) : Vector(Allocator()) {}

        explicit Vector(const Allocator& alloc_) : alloc(alloc_) {}

        explicit Vector(std::size_t cnt, const Allocator& alloc_ = Allocator()) : alloc(alloc_), sz(cnt), cap(cnt) {
            vec = alloc.allocate(cnt);
            for (std::size_t i = 0; i < cnt; i++) {
                *(vec + i) = T();
            }
        }

        Vector(std::size_t cnt, const T& val, const Allocator& alloc_ = Allocator()) : alloc(alloc_), sz(cnt), cap(cnt) {
            vec = alloc.allocate(cnt);
            for (std::size_t i = 0; i < cnt; i++) {
                *(vec + i) = val;
            }
        }

        template <class InputIt>
        Vector(InputIt first, InputIt last, const Allocator& alloc_ = Allocator()) :
        alloc(alloc_), sz(distance(first, last)), cap(distance(first, last)) {
            vec = alloc.allocate(cap);
            InputIt tmp = first;
            for (std::size_t i = 0; i < cap; i++) {
                *(vec + i) = *tmp;
                ++tmp;
            }
        }

        Vector(const Vector& other) : alloc(other.alloc), sz(other.sz), cap(other.cap) {
            vec = alloc.allocate(cap);
            for (std::size_t i = 0; i < sz; i++) {
                *(vec + i) =  other.vec[i];
            }
        }

        Vector(Vector&& other) noexcept : alloc(other.alloc), vec(other.vec), sz(other.sz), cap(other.cap) {
            other.vec = nullptr;
            other.sz = 0;
            other.cap = 0;
        }

        Vector(const Vector& other, const Allocator& alloc_) : alloc(alloc_), sz(other.sz), cap(other.cap) {
            vec = alloc.allocate(cap);
            for (std::size_t i = 0; i < sz; i++) {
                *(vec + i) = other.vec[i];
            }
        }

        Vector(Vector&& other, const Allocator& alloc_) noexcept :
        alloc(alloc_), vec(other.vec), sz(other.sz), cap(other.cap) {
            other.vec = nullptr;
            other.sz = 0;
            other.cap = 0;
        }

        Vector(std::initializer_list<T> init, const Allocator& alloc_ = Allocator()) :
        alloc(alloc_), sz(init.size()), cap(init.size()) {
            vec = alloc.allocate(cap);
            for (std::size_t i = 0; i < sz; i++) {
                *(vec + i) = *(init.begin() + i);
            }
        }

        ~Vector() final {
            if (vec != nullptr) {
                alloc.deallocate(vec, cap);
                cap = 0;
                sz = 0;
            }
        }

        Vector& operator=(const Vector& other) {
            if (this != &other) {
                if (other.alloc != alloc || other.sz > cap) {
                    clear_all();
                    alloc = other.alloc;
                    sz = other.sz;
                    cap = other.cap;
                    vec = alloc.allocate(cap);
                    for (std::size_t i = 0; i < sz; i++) {
                        *(vec + i) = other.vec[i];
                    }
                } else {
                    sz = other.sz;
                    for (std::size_t i = 0; i < sz; i++) {
                        *(vec + i) = other.vec[i];
                    }
                }
            }
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                clear_all();
                alloc = other.alloc;
                vec = other.vec;
                sz = other.sz;
                cap = other.cap;
                other.vec = nullptr;
                other.sz = 0;
                other.cap = 0;
            }
            return *this;
        }

        Vector& operator=(std::initializer_list<T> init) {
            if (cap < init.size()) {
                clear_all();
                cap = init.size();
                vec = alloc.allocate(cap);
            }
            sz = init.size();
            for (std::size_t i = 0; i < sz; i++) {
                *(vec + i) = *(init.begin() + i);
            }
            return *this;
        }

        iterator begin() noexcept {
            return iterator(vec);
        }
        ConstIterator cbegin() noexcept {
            return ConstIterator(vec);
        }
        ReverseIterator rbegin() noexcept {
            return ReverseIterator(vec + sz - 1);
        }
        ConstReverseIterator crbegin() noexcept {
            return ConstReverseIterator(vec + sz - 1);
        }
        iterator end() noexcept {
            return iterator(vec + sz);
        }
        ConstIterator cend() noexcept {
            return ConstIterator(vec + sz);
        }
        ReverseIterator rend() noexcept {
            return ReverseIterator(vec - 1);
        }
        ConstReverseIterator crend() noexcept {
            return ConstReverseIterator(vec - 1);
        }

        iterator begin() const noexcept {
            return iterator(vec);
        }
        ConstIterator cbegin() const noexcept {
            return ConstIterator(vec);
        }
        ReverseIterator rbegin() const noexcept {
            return ReverseIterator(vec + sz - 1);
        }
        ConstReverseIterator crbegin() const noexcept {
            return ConstIterator(vec + sz - 1);
        }
        iterator end() const noexcept {
            return iterator(vec + sz);
        }
        ConstIterator cend() const noexcept {
            return ConstIterator(vec + sz);
        }
        ReverseIterator rend() const noexcept {
            return ReverseIterator(vec - 1);
        }
        ConstReverseIterator crend() const noexcept {
            return ConstReverseIterator(vec - 1);
        }

        T& operator[](std::size_t idx) {
            if (idx >= sz) {
                throw std::out_of_range("Vector index out of range");
            }
            return vec[idx];
        }
        const T& operator[](std::size_t idx) const {
            if (idx >= sz) {
                throw std::out_of_range("Vector index out of range");
            }
            return const_cast<T&>(vec[idx]);
        }

        T& at(std::size_t idx) {
            if (idx >= sz) {
                throw std::out_of_range("Vector index out of range");
            }
            return vec[idx];
        }
        const T& at(std::size_t idx) const {
            if (idx >= sz) {
                throw std::out_of_range("Vector index out of range");
            }
            return const_cast<T&>(vec[idx]);
        }

        T& front() {
            if (sz == 0) {
                throw std::out_of_range("Vector is empty");
            }
            return vec[0];
        }
        const T& front() const {
            if (sz == 0) {
                throw std::out_of_range("Vector is empty");
            }
            return const_cast<T&>(vec[0]);
        }

        T& back() {
            if (sz == 0) {
                throw std::out_of_range("Vector is empty");
            }
            return vec[sz - 1];
        }
        const T& back() const {
            if (sz == 0) {
                throw std::out_of_range("Vector is empty");
            }
            return const_cast<T&>(vec[sz - 1]);
        }

        T* data() noexcept {
            return vec;
        }
        const T* data() const noexcept {
            return const_cast<T*>(vec);
        }

        [[nodiscard]] bool empty() const noexcept final {
            return sz == 0;
        }
        [[nodiscard]] std::size_t size() const noexcept final {
            return sz;
        }
        [[nodiscard]] std::size_t capacity() const noexcept {
            return cap;
        }
        [[nodiscard]] std::size_t max_size() const noexcept final {
            return std::numeric_limits<std::size_t>::max() / sizeof(T);
        }

        void reserve(std::size_t new_cap) {
            if (new_cap > cap) {
                T* new_vec = alloc.allocate(new_cap);
                for (std::size_t i = 0; i < sz; i++) {
                    *(new_vec + i) = vec[i];
                }
                if (vec != nullptr) {
                    alloc.deallocate(vec, cap);
                }
                vec = new_vec;
                cap = new_cap;
            }
        }

        void shrink_to_fit() noexcept {
            if (sz != cap) {
                T* new_vec = alloc.allocate(sz);
                for (std::size_t i = 0; i < sz; i++) {
                    *(new_vec + i) = vec[i];
                }
                if (vec != nullptr) {
                    alloc.deallocate(vec, cap);
                }
                vec = new_vec;
                cap = sz;
            }
        }

        void clear() noexcept {
            sz = 0;
        }

        void push_back(const T& val) {
            if (sz == cap) {
                if (cap == 0) {
                    reserve(2);
                } else {
                    reserve(cap * 2);
                }
            }
            *(vec + sz) =  val;
            ++sz;
        }

        void push_back(T&& val) {
            if (sz == cap) {
                if (cap == 0) {
                    reserve(2);
                } else {
                    reserve(cap * 2);
                }
            }
            *(vec + sz) = std::move(val);
            ++sz;
        }

        void pop_back() {
            if (sz == 0) {
                throw std::out_of_range("Vector is empty");
            }
            --sz;
        }

        iterator insert(const iterator pos, const T& val) {
            std::size_t ind = distance(begin(), pos);
            if (ind > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            if (sz == cap) {
                if (cap == 0) {
                    reserve(2);
                } else {
                    reserve(cap * 2);
                }
            }
            for (std::size_t i = sz; i > ind; i--) {
                *(vec + i) = std::move(*(vec + i - 1));
            }
            *(vec + ind) = val;
            ++sz;
            return iterator(vec + ind);
        }

        iterator insert(const iterator pos, T&& val) {
            std::size_t ind = distance(begin(), pos);
            if (ind > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            if (sz == cap) {
                if (cap == 0) {
                    reserve(2);
                } else {
                    reserve(cap * 2);
                }
            }
            for (std::size_t i = sz; i > ind; i--) {
                *(vec + i) = std::move(*(vec + i - 1));
            }
            *(vec + ind) = std::move(val);
            ++sz;
            return iterator(vec + ind);
        }

        iterator insert(const iterator pos, std::size_t cnt, const T& val) {
            std::size_t ind = distance(begin(), pos);
            if (ind > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            if (sz + cnt > cap) {
                if (cnt < cap) {
                    reserve(cap * 2);
                } else {
                    reserve(cap + cnt + 1);
                }
            }
            for (std::size_t i = sz; i > ind; i--) {
                *(vec + i + cnt - 1) = std::move(*(vec + i - 1));
            }
            for (std::size_t i = 0; i < cnt; i++) {
                *(vec + ind + i) = val;
            }
            sz += cnt;
            return iterator(vec + ind);
        }

        template< class InputIt >
        iterator insert(const iterator pos, InputIt first, InputIt last) {
            std::size_t cnt = distance(first, last);
            std::size_t ind = distance(begin(), pos);
            if (ind > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            if (sz + cnt > cap) {
                if (cnt < cap) {
                    reserve(cap * 2);
                } else {
                    reserve(cap + cnt + 1);
                }
            }
            for (std::size_t i = sz; i > ind; i--) {
                *(vec + i + cnt - 1) = std::move(*(vec + i - 1));
            }
            for (std::size_t i = 0; i < cnt; i++) {
                *(vec + ind + i) = *(first + i);
            }
            sz += cnt;
            return iterator(vec + ind);
        }

        iterator insert(const iterator pos, std::initializer_list<T> ilist) {
            std::size_t cnt = ilist.size();
            std::size_t ind = distance(begin(), pos);
            if (ind > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            if (sz + cnt > cap) {
                if (cnt < cap) {
                    reserve(cap * 2);
                } else {
                    reserve(cap + cnt + 1);
                }
            }
            for (std::size_t i = sz; i > ind; i--) {
                *(vec + i + cnt - 1) = std::move(*(vec + i - 1));
            }
            for (std::size_t i = 0; i < cnt; i++) {
                *(vec + ind + i) = *(ilist.begin() + i);
            }
            sz += cnt;
            return iterator(vec + ind); //xz
        }

        iterator erase(iterator pos) {
            std::size_t ind = distance(begin(), pos);
            if (sz == 0 || ind >= sz) {
                throw std::out_of_range("Vector index out of range");
            }
            for (std::size_t i = ind; i < sz - 1; ++i) {
                *(vec + i) = std::move(*(vec + i + 1));
            }
            --sz;
            return pos;
        }

        iterator erase(const iterator first, const iterator last) {
            std::size_t cnt = distance(first, last);
            const std::size_t ind = distance(begin(), first);
            if (ind + cnt > sz) {
                throw std::out_of_range("Vector index out of range");
            }
            for (std::size_t i = ind; i < sz - cnt; ++i) {
                *(vec + i) = std::move(*(vec + i + cnt));
            }
            sz -= cnt;
            return first;
        }

        void resize(std::size_t new_size) {
            if (sz > new_size) {
                erase(begin() + new_size, end());
            }
            if (sz < new_size) {
                if (new_size > cap) {
                    reserve(new_size * 2);
                }
                for (std::size_t i = sz; i < new_size; ++i) {
                    *(vec + i) = T();
                }
            }
            sz = new_size;
        }
        void resize(std::size_t new_size, const T& val) {
            if (sz > new_size) {
                erase(begin() + new_size, end());
            }
            if (sz < new_size) {
                if (new_size > cap) {
                    reserve(new_size * 2);
                }
                for (std::size_t i = sz; i < new_size; ++i) {
                    *(vec + i) = val;
                }
            }
            sz = new_size;
        }

        void swap(Vector& other) noexcept {
            std::swap(vec, other.vec);
            std::swap(cap, other.cap);
            std::swap(sz, other.sz);
            std::swap(alloc, other.alloc);
        }

        bool operator==(const Vector &other) const {
            if (sz != other.sz) {
                return false;
            }
            for (std::size_t i = 0; i < sz; ++i) {
                if (vec[i] != other.vec[i]) {
                    return false;
                }
            }
            return true;
        }
        bool operator!=(const Vector &other) const {
            return !(*this == other);
        }
        bool operator<(const Vector &other) const {
            const std::size_t less_sz = (sz > other.sz) ? other.sz : sz;

            for (std::size_t i = 0; i < less_sz; ++i) {
                if (vec[i] < other.vec[i]) {
                    return true;
                }
                if (vec[i] > other.vec[i]) {
                    return false;
                }
            }
            return sz < other.sz;
        }
        bool operator<=(const Vector &other) const {
            return (*this < other || *this == other);
        }
        bool operator>(const Vector &other) const {
            return !(*this <= other);
        }
        bool operator>=(const Vector &other) const {
            return !(*this < other);
        }
#if __cplusplus >= 202002L
        int operator<=>(const Vector &other) const {
            if (*this == other) {
                return 0;
            }
            if (*this < other) {
                return -1;
            }
            return 1;
        }
#endif
    };
}