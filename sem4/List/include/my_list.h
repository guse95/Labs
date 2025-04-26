#pragma once
#include <limits>
#include "my_container.h"

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
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T*;
            using reference = T&;

            ListIter() : iter(nullptr) {}
            explicit ListIter(node *iter) : iter(iter) {}
            ListIter(const ListIter &other) : iter(other.iter) {}
            ListIter(ListIter &&other) noexcept : iter(other.iter) {}

            ListIter& operator=(const ListIter &other) = default;
            ListIter& operator++() {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                this->iter = this->iter->next;
                return *this;
            }
            ListIter operator++(int) {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                ListIter temp = *this;
                this->iter = this->iter->next;
                return temp;
            }
            ListIter& operator--() {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                this->iter = this->iter->prev;
                return *this;
            }
            ListIter operator--(int) {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
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
        };

        template <class IterType>
        class ListReverseIter
        {
        protected:
            friend class List;
            node *iter = nullptr;
        public:

            ListReverseIter() : iter(nullptr) {}
            explicit ListReverseIter(node *iter) : iter(iter) {}
            ListReverseIter(const ListReverseIter &other) = default;
            ListReverseIter(ListReverseIter &&other) = default;

            ListReverseIter& operator=(const ListReverseIter &other) = default;
            ListReverseIter& operator++() {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                this->iter = this->iter->prev;
                return *this;
            }
            ListReverseIter operator++(int) {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                ListReverseIter temp = *this;
                this->iter = this->iter->prev;
                return temp;
            }
            ListReverseIter& operator--() {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
                this->iter = this->iter->next;
                return *this;
            }
            ListReverseIter operator--(int) {
                if (!this->iter) throw std::runtime_error("Dereferencing null iterator");
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
        };

        void destroy_node(node* n) noexcept {
            std::allocator_traits<AllocNode>::destroy(alloc, n);
            std::allocator_traits<AllocNode>::deallocate(alloc, n, 1);
        }

        node* create_node(const T& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            try {
                std::allocator_traits<AllocNode>::construct(alloc, new_node, val);
                return new_node;
            } catch (...) {
                std::allocator_traits<AllocNode>::deallocate(alloc, new_node, 1);
                throw std::exception();
            }
        }

        node* create_node(const T&& val) {
            node* new_node = std::allocator_traits<AllocNode>::allocate(alloc, 1);
            try {
                std::allocator_traits<AllocNode>::construct(alloc, new_node, std::move(val));
                return new_node;
            } catch (...) {
                std::allocator_traits<AllocNode>::deallocate(alloc, new_node, 1);
                throw std::exception();
            }
        }

    public:

        using iterator = ListIter<T>;
        using ConstIterator = ListIter<const T>;
        using ReverseIterator = ListReverseIter<T>;
        using ConstReverseIterator = ListReverseIter<const T>;

        List() : List(Allocator()) {}

        explicit List(const Allocator& alloc_) : Head(nullptr), Tail(nullptr), len(0), alloc(alloc_) {}

        explicit List(const std::size_t cnt, const Allocator& alloc_ = Allocator()) :
        len(cnt), alloc(alloc_) {
            Head = create_node(0);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                tmp->next = create_node(0);
                tmp->next->prev = tmp;
                tmp = tmp->next;
            }
            Tail = tmp;
            Tail->next = nullptr;
        }

        List(const std::size_t cnt, const T& val_, const Allocator& alloc_ = Allocator()) :
        len(cnt), alloc(alloc_) {
            Head = create_node(val_);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                tmp->next = create_node(val_);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp = tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other) :
        len(other.len), alloc(other.alloc) {
            Head = create_node(other.Head->val);
            Head->prev = nullptr;
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = create_node(other_tmp->val);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other, const Allocator& alloc_ ) :
        len(other.len), alloc(alloc_) {
            Head = create_node(other.Head->val);
            Head->prev = nullptr;
            node *tmp = Head;
            node *other_tmp = other.Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = create_node(other_tmp->val);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
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
            Head = create_node(*first++);
            Head->prev = nullptr;
            len = 1;
            node *tmp = Head;
            while (first != last) {
                node* new_node = create_node(*first);
                ++first;
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

        ~List() override {
            node* cur = Head;
            while (cur != nullptr) {
                node* next = cur->next;
                destroy_node(cur);
                cur = next;
            }
            Head = nullptr;
            Tail = nullptr;
            len = 0;
        }

        T& front() {
            if (empty()) throw std::out_of_range("List is empty");
            return Head->val;
        }
        T& back() {
            if (empty()) throw std::out_of_range("List is empty");
            return Tail->val;
        }
        const T& front() const {
            if (empty()) throw std::out_of_range("List is empty");
            return Head->val;
        }
        const T& back() const {
            if (empty()) throw std::out_of_range("List is empty");
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
        ConstReverseIterator crbegin() {
            return ConstReverseIterator(Tail);
        }

        iterator end() {
            return iterator(nullptr);
        }
        ConstIterator cend() {
            return ConstIterator(nullptr);
        }
        ReverseIterator rend() {
            return ReverseIterator(nullptr);
        }
        ConstReverseIterator crend() {
            return ConstReverseIterator(nullptr);
        }

        iterator begin() const {
            return iterator(Head);
        }
        ConstIterator cbegin() const {
            return ConstIterator(Head);
        }
        ReverseIterator rbegin() const {
            return ReverseIterator(Tail);
        }
        ConstReverseIterator crbegin() const {
            return ConstReverseIterator(Tail);
        }
        iterator end() const {
            return iterator(nullptr);
        }
        ConstIterator cend() const {
            return ConstIterator(nullptr);
        }
        ReverseIterator rend() const {
            return ReverseIterator(nullptr);
        }
        ConstReverseIterator crend() const {
            return ConstReverseIterator(nullptr);
        }

        [[nodiscard]] bool empty() const noexcept final {
            return Head == nullptr;
        }
        [[nodiscard]] std::size_t size() const noexcept final {
            return len;
        }
        [[nodiscard]] std::size_t max_size() const noexcept final {
            return std::numeric_limits<std::size_t>::max() / sizeof(node);
        }

        void clear() {
            node* cur = Head;
            while (cur != nullptr) {
                node* next = cur->next;
                destroy_node(cur);
                cur = next;
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
                Tail = new_node;
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
            node* new_node = create_node(std::move(val));
            new_node->next = pos.iter;
            if (pos == end()) {
                new_node->prev = Tail;
                if (Tail != nullptr) {
                    Tail->next = new_node;
                }
                Tail = new_node;
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
            if (tmp != nullptr) {
                return iterator(tmp->next);
            }
            return begin();
        }

        template<class InputIt>
        iterator insert(const iterator pos, InputIt first, InputIt last) {
            if (first == last) {
                return pos;
            }
            iterator tmp = insert(pos, *first);
            ++first;

            while (first != last) {
                insert(pos, *first);
                ++first;
            }
            return tmp;
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

            if (to_del == Head) {
                Head = next_node;
                if (Head != nullptr) {
                    Head->prev = nullptr;
                }
            } else {
                to_del->prev->next = to_del->next;
            }
            if (to_del == Tail) {
                Tail = to_del->prev;
                if (Tail != nullptr) {
                    Tail->next = nullptr;
                }
            } else {
                next_node->prev = to_del->prev;
            }

            destroy_node(to_del);
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
            } else {
                first.iter->prev->next = last.iter;
            }
            if (last == end()) {
                Tail = first.iter->prev;
                if (Tail != nullptr) {
                    Tail->next = nullptr;
                }
            }
            else {
                last.iter->prev = first.iter->prev;
            }
            node* tmp = first.iter;
            while (tmp != last.iter) {
                node* next_it = tmp->next;
                destroy_node(tmp);
                tmp = next_it;
                --len;
            }
            return last;
        }

        void push_back(const T& val) {
            node* new_node = create_node(val);
            new_node->next = nullptr;

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
            node* new_node = create_node(std::move(val));
            new_node->next = nullptr;

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
            erase(iterator(Tail));
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
            node* new_node = create_node(std::move(val));
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
            erase(begin());
        }

        List& operator=(const List& other) {
            if (this == &other) {
                return *this;
            }

            if (alloc == other.alloc) {
                if (Head == nullptr) {
                    for (const T& val : other) {
                        push_back(val);
                    }
                    return *this;
                }
                Head->val = other.Head->val;
                node *tmp = Head;
                node *other_tmp = other.Head->next;

                for (std::size_t i = 1; i < other.len; i++) {
                    if (i >= len) {
                        push_back(other_tmp->val);
                    } else {
                        tmp->next->val = other_tmp->val;
                    }
                    tmp = tmp->next;
                    other_tmp = other_tmp->next;
                }
                Tail = tmp;

                node *tmp_del = tmp->next;
                while (tmp_del != nullptr) {
                    node *next = tmp_del->next;
                    destroy_node(tmp_del);
                    tmp_del = next;
                }

                len = other.len;
                return *this;
            }
            // deallocate by old alloc
            node *tmp_d = Head;
            while (tmp_d != nullptr) {
                node *next = tmp_d->next;
                destroy_node(tmp_d);
                tmp_d = next;
            }
            // copy =)
            alloc = other.alloc;

            for (const T& val : other) {
                push_back(val);
            }
            return *this;
        }

        List& operator=(List&& other)  noexcept {
            if (this == &other) {
                return *this;
            }

            if (alloc == other.alloc && Head != nullptr) {

                Head->val = other.Head->val;
                node *tmp = Head;
                node *other_tmp = other.Head->next;

                for (std::size_t i = 1; i < other.len && i < len; i++) {
                    tmp->next->val = std::move(other_tmp->val);
                    tmp = tmp->next;
                    other_tmp = other_tmp->next;
                }
                // if other.len == len
                Tail = tmp;
                // if other.len > len
                if (tmp->next == nullptr && other_tmp != nullptr) {
                    tmp->next = other_tmp;
                    other_tmp->prev = tmp;
                }
                // if other.len < len
                node *tmp_del = tmp->next;
                while (tmp_del != nullptr) {
                    node *next = tmp_del->next;
                    destroy_node(tmp_del);
                    tmp_del = next;
                }
            } else {
                // deallocate by old alloc
                node *tmp_d = Head;
                while (tmp_d != nullptr) {
                    node *next = tmp_d->next;
                    destroy_node(tmp_d);
                    tmp_d = next;
                }
                // move =)
                alloc = std::move(other.alloc);
                Head = other.Head;
                Tail = other.Tail;
            }

            len = other.len;
            other.len = 0;
            other.Head = nullptr;
            other.Tail = nullptr;
            return *this;
        }

        List& operator=(std::initializer_list<T> ilist) {
            auto ptr = ilist.begin();
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
            if (tmp != nullptr) {
                Tail = tmp->prev;
                node *tmp_del = tmp;
                while (tmp_del != nullptr) {
                    node *next = tmp_del->next;
                    destroy_node(tmp_del);
                    tmp_del = next;
                }
            }
            if (ilist.end() != ptr) {
                while (ilist.end() != ptr) {
                    push_back(*ptr);
                    ++ptr;
                }
            }
            return *this;
        }

        void resize(std::size_t cnt, const T& val) {
            if (cnt == len) {
                return;
            }
            if (cnt < len) {
                iterator from = begin();
                for (std::size_t i = cnt; i < cnt; i++) {
                    ++from;
                }
                erase(iterator(begin().iter + cnt), end());
                return;
            }
            while (len != cnt) {
                push_back(val);
            }
        }

        void resize(std::size_t cnt) {
            if (cnt == len) {
                return;
            }
            if (cnt < len) {
                iterator from = begin();
                for (std::size_t i = 0; i < cnt; i++) {
                    ++from;
                }
                erase(from, end());
                return;
            }
            while (len != cnt) {
                push_back(0);
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
            node* l = Head;
            node* r = other.Head;
            for (std::size_t i = 0; i < len && i < other.len; i++) {
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
        int operator<=>( const List& other ) const {
            if (*this > other) {
                return 1;
            }
            if (*this == other) {
                return 0;
            }
            return -1;
        }
#endif
    };
}