#include "my_container.h"

namespace my_container {

    template <class T, class Allocator = std::allocator<T>>
    class List : public Container<T> {

    protected:

        struct node {
            T val;
            node *next;
            node *prev;
        };

        node *Head;
        node *Tail;
        std::size_t len;
        Allocator alloc;
        // using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
        // AllocNode allocNode;
        
        template <class IterType>
        class ListIter
        {
        protected:
            friend class List;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit ListIter(NoConstIterT *iter) : iter(iter) {}
            ListIter(const ListIter &other) = default;
            ListIter(ListIter &&other) = default;

        public:
            ListIter operator++() {
                this->iter = this->iter->next;
                return *this;
            }
            ListIter operator++(int) {
                ListIter temp = *this;
                this->iter = this->iter->next;
                return temp;
            }
            ListIter operator--() {
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
            IterType operator*() const {
                return *this->iter->val;
            }
        };

        template <class IterType>
        class ListReverseIter
        {
        protected:
            friend class Array;
            using NoConstIterT = std::remove_const_t<IterType>;
            NoConstIterT *iter = nullptr;
            explicit ListReverseIter(NoConstIterT *iter) : iter(iter) {}
            ListReverseIter(const ListReverseIter &other) = default;
            ListReverseIter(ListReverseIter &&other) = default;

        public:
            ListReverseIter operator++() {
                this->iter = this->iter->prev;
                return *this;
            }
            ListReverseIter operator++(int) {
                ListIter temp = *this;
                this->iter = this->iter->prev;
                return temp;
            }
            ListReverseIter operator--() {
                this->iter = this->iter->next;
                return *this;
            }
            ListReverseIter operator--(int) {
                ListIter temp = *this;
                this->iter = this->iter->next;
                return temp;
            }
            bool operator==(const ListReverseIter &other) const {
                return this->iter == other.iter;
            }
            bool operator!=(const ListReverseIter &other) const {
                return this->iter != other.iter;
            }
            IterType operator*() const {
                return *this->iter->val;
            }
        };

    public:

        using iterator = ListIter<T>;
        using ConstIterator = ListIter<const T>;
        using ReverseIterator = ListReverseIter<T>;
        using ConstReverseIterator = ListReverseIter<const T>;

        List() : List(Allocator()) {}

        explicit List(const Allocator& alloc_) :
        len(0) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = nullptr;
            Tail = nullptr;
        }

        explicit List(const std::size_t cnt, Allocator& alloc_ = Allocator()) :
        len(cnt) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = this->alloc.allocate(1);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                this->tmp->next = alloc.allocate(1);
                this->tmp->next->prev = tmp;
                this->tmp->next->next = nullptr;
                tmp = tmp->next;
            }
            Tail = tmp;
        }

        List(const std::size_t cnt, const T& val_, const Allocator& alloc_ = Allocator()) :
        len(cnt) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = this->alloc.allocate(1);
            Head->prev = nullptr;
            node *tmp = Head;
            for (std::size_t i = 1; i < cnt; i++) {
                tmp->next = alloc.allocate(1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = val_;
                tmp = tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other) : len(other->len), alloc(other.alloc) {
            Head = alloc.allocate(1);
            Head->prev = nullptr;
            Head->val = other.Head->val;
            node *tmp = Head;
            node *other_tmp = other->Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = alloc.allocate(1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = other_tmp->val;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
        }

        List(const List& other, const Allocator& alloc_ = Allocator()) :
        len(other->len) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = alloc.allocate(1);
            Head->prev = nullptr;
            Head->val = other.Head->val;
            node *tmp = Head;
            node *other_tmp = other->Head->next;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = alloc.allocate(1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = other_tmp->val;
                tmp = tmp->next;
                other_tmp = other_tmp->next;
            }
            Tail = tmp;
        }

        List(List&& other)  noexcept :
        Head(other.Head), Tail(other.Tail), len(other.len), alloc(other.alloc) {
            other.Head = nullptr;
            other.Tail = nullptr;
            other.len = 0;
            other.alloc = nullptr;
        }

        List(List&& other, const Allocator& alloc_ = Allocator())  noexcept :
        Head(other.Head), Tail(other.Tail), len(other.len) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            other.Head = nullptr;
            other.Tail = nullptr;
            other.len = 0;
            other.alloc = nullptr;
        }

        template< class InputIt >
        List( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) {
            len = std::distance(first, last);
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = alloc.allocate(1);
            Head->prev = nullptr;
            Head->val = *first;
            node *tmp = Head;
            node *other_tmp = ++first;
            for (std::size_t i = 1; i < len; i++) {
                tmp->next = alloc.allocate(1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp->next->val = *other_tmp++;
                tmp = tmp->next;
                // ++other_tmp;
            }
            Tail = tmp;
        }

        List(std::initializer_list<T> init, const Allocator& alloc = Allocator()) :
        List(init.begin(), init.end(), alloc) {}

        ~List() final {
            for (node *tmp = Head; tmp != nullptr; tmp = tmp->next) {
                alloc.deallocate(tmp, 1);
            }
        }

        List& operator=(const List& other) {

        }
    };
}