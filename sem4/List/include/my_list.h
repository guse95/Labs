#pragma once
#include <limits>
#include "../src/my_container.h"

namespace my_container {

    template <class T, class Allocator = std::allocator<T>>
    class List : public Container<T> {

    protected:

        struct node {
            T val;
            node *next;
            node *prev;

            node() : next(nullptr), prev(nullptr) {}
            explicit node(const T& val) : val(val), next(nullptr), prev(nullptr) {}
            explicit node(T&& val) : val(std::move(val)), next(nullptr), prev(nullptr) {}
        };

        node *Head;
        node *Tail;
        std::size_t len;
        using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
        AllocNode alloc;
        
        template <class IterType>
        class ListIter
        {
        protected:
            friend class List;
            node *iter = nullptr;
        public:
            explicit ListIter(node *iter) : iter(iter) {}
            ListIter(const ListIter &other) : iter(other.iter) {}
            ListIter(ListIter &&other) noexcept : iter(other.iter) {}

            ListIter& operator=(const ListIter &other) = default;
            ListIter& operator++() {
                this->iter = this->iter->next;
                return *this;
            }
            ListIter operator++(int) {
                ListIter temp = *this;
                this->iter = this->iter->next;
                return temp;
            }
            ListIter& operator--() {
                this->iter = this->iter->prev;
                return *this;
            }
            ListIter operator--(int) {
                ListIter temp = *this;
                this->iter = this->iter->prev;
                return temp;
            }
            bool operator==(const ListIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const ListIter &other) const {
                return this->iter != other.iter;
            }
            IterType& operator*() const {
                if (!iter) throw std::runtime_error("Dereferencing null iterator");
                return this->iter->val;
            }
            IterType* operator->() const {
                if (!iter) throw std::runtime_error("Dereferencing null iterator");
                return &this->iter->val;
            }
        };

        template <class IterType>
        class ListReverseIter
        {
        protected:
            friend class List;
            node *iter = nullptr;
        public:
            explicit ListReverseIter(node *iter) : iter(iter) {}
            ListReverseIter(const ListReverseIter &other) = default;
            ListReverseIter(ListReverseIter &&other) = default;

            ListReverseIter& operator=(const ListReverseIter &other) = default;
            ListReverseIter& operator++() {
                this->iter = this->iter->prev;
                return *this;
            }
            ListReverseIter operator++(int) {
                ListReverseIter temp = *this;
                this->iter = this->iter->prev;
                return temp;
            }
            ListReverseIter& operator--() {
                this->iter = this->iter->next;
                return *this;
            }
            ListReverseIter operator--(int) {
                ListReverseIter temp = *this;
                this->iter = this->iter->next;
                return temp;
            }
            bool operator==(const ListReverseIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const ListReverseIter &other) const {
                return this->iter != other.iter;
            }
            IterType& operator*() const {
                if (!iter) throw std::runtime_error("Dereferencing null iterator");
                return this->iter->val;
            }
            IterType* operator->() const {
                if (!iter) throw std::runtime_error("Dereferencing null iterator");
                return &this->iter->val;
            }
        };

    public:

        using iterator = ListIter<T>;
        using ConstIterator = ListIter<const T>;
        using ReverseIterator = ListReverseIter<T>;
        using ConstReverseIterator = ListReverseIter<const T>;

        List() : List(Allocator()) {}

        explicit List(const Allocator& alloc_) : Head(nullptr), Tail(nullptr), len(0), alloc(alloc_) {}

        explicit List(const std::size_t cnt, const Allocator& alloc_ = Allocator()) :
        len(cnt), alloc(alloc_) {
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            std::allocator_traits<AllocNode>::construct(alloc, Head);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                std::allocator_traits<AllocNode>::construct(alloc, tmp->next);
                tmp->next->prev = tmp;
                tmp = tmp->next;
            }
            Tail = tmp;
            Tail->next = nullptr;
        }

        List(const std::size_t cnt, const T& val_, const Allocator& alloc_ = Allocator()) :
        len(cnt), alloc(alloc_) {
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            std::allocator_traits<AllocNode>::construct(alloc, Head, val_);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                std::allocator_traits<AllocNode>::construct(alloc, tmp->next, val_);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp = tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other) :
        len(other.len), alloc(other.alloc) {
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            Head->prev = nullptr;
            Head->val = other.Head->val;
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = other_tmp->val;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other, const Allocator& alloc_ ) :
        len(other.len), alloc(alloc_) {
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            Head->prev = nullptr;
            Head->val = other.Head->val;
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = other_tmp->val;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
        }

        List(List&& other)  noexcept :
        Head(other.Head), Tail(other.Tail), len(other.len), alloc(std::move(other.alloc)) {
            other.Head = nullptr;
            other.Tail = nullptr;
            other.len = 0;
        }

        List(List&& other, const Allocator& alloc_ )  noexcept :
        Head(other.Head), Tail(other.Tail), len(other.len), alloc(std::move(alloc_)) {
            other.Head = nullptr;
            other.Tail = nullptr;
            other.len = 0;
        }

        template< class InputIt >
        List( InputIt first, InputIt last, const Allocator& alloc_ = Allocator() ) :
        Head(nullptr), Tail(nullptr), len(0), alloc(alloc_) {
            if (first == last) {
                return;
            }
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            Head->prev = nullptr;
            len = 1;
            Head->val = *first;
            node *tmp = Head;
            while (first != last) {
                node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                std::allocator_traits<AllocNode>::construct(alloc, new_node, *first++);

                new_node->prev = tmp;
                tmp->next = new_node;
                tmp = new_node;
                len++;
            }
            Tail = tmp;
            Tail->next = nullptr;
        }

        List(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
        List(init.begin(), init.end(), alloc) {}

        ~List() final {
            node *tmp = Head;
            while (tmp != nullptr) {
                node *next = tmp->next;
                std::allocator_traits<AllocNode>::destroy(alloc, tmp);
                std::allocator_traits<AllocNode>::deallocate(alloc, tmp, 1);
                tmp = next;
            }
            Head = nullptr;
            Tail = nullptr;
            len = 0;
        }

        List& operator=(const List& other) {
            if (this == &other) {
                return *this;
            }
            if (alloc == other.alloc) {
                if (Head == nullptr) {
                    Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                    Head->prev = nullptr;
                }
                Head->val = other.Head->val;
                node *tmp = Head;
                node *other_tmp = other.Head->next;
                for (std::size_t i = 1; i < other.len; i++) {
                    if (i >= len) {
                        tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                        tmp->next->prev = tmp;
                        tmp->next->next = nullptr;
                    }
                    tmp->next->val = other_tmp->val;
                    tmp = tmp->next;
                    other_tmp = other_tmp->next;
                }
                Tail = tmp;

                node *tmp_del = tmp;
                while (tmp_del != nullptr) {
                    node *next = tmp_del->next;
                    std::allocator_traits<AllocNode>::deallocate(alloc, tmp_del, 1);
                    tmp_del = next;
                }

                len = other.len;
                return *this;
            }
            // deallocate by old alloc
            node *tmp_d = Head;
            while (tmp_d != nullptr) {
                node *next = tmp_d->next;
                std::allocator_traits<AllocNode>::deallocate(alloc, tmp_d, 1);
                tmp_d = next;
            }
            // copy constructor =)
            // *this = new List(other);
            alloc = other.alloc;
            len = other.len;
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            Head->prev = nullptr;
            Head->val = other.Head->val;
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = other_tmp->val;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
            return *this;
        }

        List& operator=(List&& other) noexcept {
            if (this == &other) {
                return *this;
            }
            if (alloc == other.alloc) {
                if (Head == nullptr) {
                    Head = other.Head;
                    Tail = other.Tail;
                    len = other.len;
                    return *this;
                }
                Head->val = other.Head->val;
                node *tmp = Head;
                node *other_tmp = other.Head->next;
                for (std::size_t i = 1; i < len && i < other.len; i++) {
                    tmp->next->val = other_tmp->val;
                    tmp = tmp->next;
                    other_tmp = other_tmp->next;
                }
                if (other_tmp == nullptr && tmp->next != nullptr) {
                    node *tmp_del = tmp->next;
                    while (tmp_del != nullptr) {
                        node *next = tmp_del->next;
                        std::allocator_traits<AllocNode>::deallocate(alloc, tmp_del, 1);
                        tmp_del = next;
                    }
                    Tail = tmp;
                    tmp->next = nullptr;
                } else {
                    Tail = other.Tail;
                }
                tmp->next = other_tmp;
                len = other.len;
                return *this;
            }
            // deallocate by old alloc
            node *tmp_d = Head;
            while (tmp_d != nullptr) {
                node *next = tmp_d->next;
                std::allocator_traits<AllocNode>::deallocate(alloc, tmp_d, 1);
                tmp_d = next;
            }
            // copy constructor =))
            alloc = std::move(other.alloc);
            len = other.len;
            Head = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            Head->prev = nullptr;
            Head->val = std::move(other.Head->val);
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = std::move(other_tmp->val);
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
            return *this;
        }
        List& operator=(std::initializer_list<T> ilist) {
            T* ptr = ilist.begin();
            if (Head == nullptr) {
                List(ilist.begin(), ilist.end(), alloc);
                return *this;
            }
            node* tmp = Head;
            while (ilist.end() != ptr && tmp != nullptr) {
                tmp->val = *ptr;
                tmp = tmp->next;
                ++ptr;
            }
            if (ilist.end() != ptr) {
                tmp = Tail;
                while (ilist.end() != ptr) {
                    tmp->next = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                    tmp->next->prev = tmp;
                    tmp->next->next = nullptr;
                    tmp->next->val = ptr;
                    tmp = tmp->next;
                    ++ptr;
                }
                Tail = tmp;
            }
            return *this;
        }

        T& front() {
            return Head->val;
        }
        T& back() {
            return Tail->val;
        }
        const T& front() const {
            return Head->val;
        }
        const T& back() const {
            return Tail->val;
        }

        iterator begin() {
            return iterator(Head);
        }
        ConstIterator cbegin() {
            return ConstIterator(Head);
        }
        ReverseIterator rbegin() {
            return ReverseIterator(Tail);
        }
        ConstReverseIterator crbegin() const {
            return ConstReverseIterator(Tail);
        }

        iterator end() {
            return iterator(Tail->next);
        }
        ConstIterator cend() {
            return ConstIterator(Tail->next);
        }
        ReverseIterator rend() {
            return ReverseIterator(Head->prev);
        }
        ConstReverseIterator crend() {
            return ConstReverseIterator(Head->prev);
        }

        [[nodiscard]] bool empty() const noexcept final {
            return len == 0;
        }
        [[nodiscard]] std::size_t size() const noexcept final {
            return len;
        }
        [[nodiscard]] std::size_t max_size() const noexcept final {
            return std::numeric_limits<std::size_t>::max() / sizeof(node);
        }

        void clear() {
            node *tmp = Head;
            while (tmp != nullptr) {
                node *next = tmp->next;
                std::allocator_traits<AllocNode>::deallocate(alloc, tmp, 1);
                tmp = next;
            }
            Head = nullptr;
            Tail = nullptr;
            len = 0;
        }

        iterator insert(const iterator pos, const T& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = val;
            new_node->next = pos.iter;
            if (pos == end()) {
                new_node->prev = Tail;
                if (Tail != nullptr) {
                    Tail->next = new_node;
                }
                new_node = Tail;
            } else {
                new_node->prev = pos.iter->prev;
                if (pos != begin()) {
                    pos.iter->prev->next = new_node;
                } else {
                    Head = new_node;
                }
                pos.iter->prev = new_node;
            }
            ++len;
            return iterator(new_node);
        }
        iterator insert(const iterator pos, const T&& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = std::move(val);
            new_node->next = pos.iter;
            if (pos == end()) {
                new_node->prev = Tail;
                if (Tail != nullptr) {
                    Tail->next = new_node;
                }
                new_node = Tail;
            } else {
                new_node->prev = pos.iter->prev;
                if (pos != begin()) {
                    pos.iter->prev->next = new_node;
                } else {
                    Head = new_node;
                }
                pos.iter->prev = new_node;
            }
            ++len;
            return iterator(new_node);
        }
        iterator insert(const iterator pos, const std::size_t cnt, const T& val) {
            if (cnt == 0) {
                return pos;
            }
            node* tmp = pos.iter->prev;
            for (std::size_t i = 0; i < cnt; i++) {
                insert(pos, val);
            }
            len += cnt;
            return iterator(tmp->next);
        }
        template<class InputIt>
        iterator insert(const iterator pos, InputIt first, InputIt last) {
            if (first == last) {
                return pos;
            }
            node* tmp = pos.iter->prev;
            InputIt* it = first;
            while (it != last) {
                insert(pos, *it);
                ++it;
                ++len;
            }
            return iterator(tmp->next);
        }
        iterator insert(const iterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        iterator erase(const iterator pos) {
            if (pos == end()) {
                throw std::out_of_range("out of range");
            }
            node* to_del = pos.iter;
            node* next_node = to_del->next;

            if (pos == begin()) {
                Head = to_del->next;
                if (Head) {
                    Head->prev = nullptr;
                }
            }
            if (to_del == Tail) {
                Tail = to_del->prev;
                if (Tail) {
                    Tail->next = nullptr;
                }
            }
            if (pos != iterator(Tail) && pos != begin()) {
                to_del->prev->next = to_del->next;
                to_del->next->prev = to_del->prev;
            }

            std::allocator_traits<AllocNode>::deallocate(alloc, to_del, 1);
            --len;
            return iterator(next_node);
        }
        iterator erase(const iterator first, const iterator last) {
            if (first == last) {
                return end();
            }
            if (first == end()) {
                throw std::out_of_range("out of range");
            }
            if (first == begin()) {
                Head = last.iter;
                if (Head != nullptr) {
                    Head->prev = nullptr;
                }
            }
            if (last == end()) {
                Tail = first.iter->prev;
                if (Tail != nullptr) {
                    Tail->next = nullptr;
                }
            }
            else {
                first.iter->prev->next = last.iter;
                last.iter->prev = first.iter->prev;
            }
            iterator it = first;
            while (it != last) {
                iterator next_it = ++it;
                --it;
                std::allocator_traits<AllocNode>::deallocate(alloc, it.iter, 1);
                it = next_it;
                --len;
            }
            return last;
        }

        void push_back(const T& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = val;
            new_node->prev = nullptr;

            if (empty()) {
                Head = new_node;
                Tail = new_node;
            } else {
                new_node->prev = Tail;
                Tail->next = new_node;
                Tail = new_node;
            }
            ++len;
        }
        void push_back(T&& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = std::move(val);
            new_node->prev = nullptr;

            if (empty()) {
                Head = new_node;
                Tail = new_node;
            } else {
                new_node->prev = Tail;
                Tail->next = new_node;
                Tail = new_node;
            }
            ++len;
        }
        void pop_back() {
            if (empty()) {
                throw std::out_of_range("list is empty");
            }
            Tail = Tail->prev;
            std::allocator_traits<AllocNode>::deallocate(alloc, Tail->next, 1);
            Tail->next = nullptr;
            if (len == 1) {
                Head = nullptr;
            }
            --len;
        }

        void push_front(const T& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = val;
            new_node->prev = nullptr;

            if (empty()) {
                Head = new_node;
                Tail = new_node;
            } else {
                new_node->next = Head;
                Head->prev = new_node;
                Head = new_node;
            }
            ++len;
        }
        void push_front(T&& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            new_node->val = std::move(val);
            new_node->prev = nullptr;

            if (empty()) {
                Head = new_node;
                Tail = new_node;
            } else {
                new_node->next = Head;
                Head->prev = new_node;
                Head = new_node;
            }
            ++len;
        }
        void pop_front() {
            if (empty()) {
                throw std::out_of_range("list is empty");
            }
            Head = Head->next;
            std::allocator_traits<AllocNode>::deallocate(alloc, Head->prev, 1);
            Head->prev = nullptr;
            if (len == 1) {
                Tail = nullptr;
            }
            --len;
        }

        void resize(std::size_t cnt, const T& val) {
            if (cnt == len) {
                return;
            }
            if (cnt < len) {
                erase(iterator(begin().iter + cnt), end());
            }
            for (std::size_t i = len; i < cnt; i++) {
                push_back(val);
            }
        }
        void resize(std::size_t cnt) {
            if (cnt == len) {
                return;
            }
            if (cnt < len) {
                erase(iterator(begin().iter + cnt), end());
            }
            while (len != cnt) {
                node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
                new_node->next = nullptr;
                new_node->prev = Tail;
                if (Head == nullptr) {
                    Head = new_node;
                }
                Tail->next = new_node;
                Tail = new_node;
                ++len;
            }
        }

        void swap(List& other) noexcept {
            node* tmp = other.Head;
            other.Head = Head;
            Head = tmp;
            tmp = other.Tail;
            other.Tail = Tail;
            Tail = tmp;
            std::size_t tm = len;
            len = other.len;
            other.len = tm;
            std::allocator<node> tmp_alloc = other.alloc;
            other.alloc = alloc;
            alloc = tmp_alloc;
        }

        bool operator==( const List& other ) const {
            if (len != other.len) {
                return false;
            }
            node *l = Head, *r = other.Head;
            while (l != nullptr) {
                if (l->val != r->val) {
                    return false;
                }
                l = l->next;
                r = r->next;
            }
            return true;
        }
        bool operator!=( const List& other ) const {
            return !(*this == other);
        }

        bool operator<( const List& other ) const {
            node* l = Head, *r = other.Head;
            for (std::size_t i = 0; i < len; i++) {
                if (l->val != r->val) {
                    return l->val < r->val;
                }
                l = l->next;
                r = r->next;
            }
            return len < other.len;
        }
        bool operator<=( const List& other ) const {
            return ((*this == other)  || (*this < other));
        }
        bool operator>( const List& other ) const {
            return !(*this <= other);
        }
        bool operator>=( const List& other ) const{
            return !(*this < other);
        }
#if __cplusplus >= 202002L
        std::weak_ordering operator<=>( const List& other ) const {
            if (*this > other) {
                return std::weak_ordering::greater;
            }
            if (*this == other) {
                return std::weak_ordering::equivalent;
            }
            return std::weak_ordering::less;
        }
#endif
    };
}