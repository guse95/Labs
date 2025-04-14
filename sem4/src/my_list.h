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
        using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
        AllocNode alloc;
        
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
            friend class List;
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
                ListReverseIter temp = *this;
                this->iter = this->iter->prev;
                return temp;
            }
            ListReverseIter operator--() {
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
                tmp->next = alloc.allocate(1);
                tmp->next->prev = tmp;
                tmp->next->next = nullptr;
                tmp = tmp->next;
            }
            Tail = tmp;
        }

        List(const std::size_t cnt, const T& val_, const Allocator& alloc_ = Allocator()) :
        len(cnt) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = allocNode;
            Head = alloc.allocate(1);
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
        Head(other.Head), Tail(other.Tail), len(other.len), alloc(std::move(other.alloc)) {
            other.Head = nullptr;
            other.Tail = nullptr;
            other.len = 0;
        }

        List(List&& other, const Allocator& alloc_ = Allocator())  noexcept :
        Head(other.Head), Tail(other.Tail), len(other.len) {
            using AllocNode = typename std::allocator_traits<Allocator>::template rebind_alloc<node>;
            AllocNode allocNode;
            alloc = std::move(allocNode);
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
            node *tmp = Head;
            while (tmp != nullptr) {
                node *next = tmp->next;
                alloc.deallocate(tmp, 1);
                tmp = next;
            }
        }

        List& operator=(const List& other) {
            if (this == &other) {
                return *this;
            }
            if (alloc == other.alloc) {
                if (Head == nullptr) {
                    Head = alloc.allocate(1);
                    Head->prev = nullptr;
                }
                Head->val = other.Head->val;
                node *tmp = Head;
                node *other_tmp = other->Head->next;
                for (std::size_t i = 1; i < other.len; i++) {
                    if (i >= len) {
                        tmp->next = alloc.allocate(1);
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
                    alloc.deallocate(tmp_del, 1);
                    tmp_del = next;
                }

                len = other.len;
                return *this;
            }
            // deallocate by old alloc
            ~List();
            // copy constructor =)
            // *this = new List(other);
            alloc = other.alloc;
            len = other.len;
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
                node *other_tmp = other->Head->next;
                for (std::size_t i = 1; i < len && i < other.len; i++) {
                    tmp->next->val = other_tmp->val;
                    tmp = tmp->next;
                    other_tmp = other_tmp->next;
                }
                if (other_tmp == nullptr && tmp->next != nullptr) {
                    node *tmp_del = tmp->next;
                    while (tmp_del != nullptr) {
                        node *next = tmp_del->next;
                        alloc.deallocate(tmp_del, 1);
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
            ~List();
            // copy constructor =))
            // *this = new List(other);
            alloc = other.alloc;
            len = other.len;
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
                    tmp->next = alloc.allocate(1);
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
            return *Head->val;
        }
        T& back() {
            return *Tail->val;
        }
        const T& front() const {
            return *Head->val;
        }
        const T& back() const {
            return *Tail->val;
        }

        iterator begin() {
            return iterator(Head);
        }
        ConstIterator сbegin() {
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
            return ReverseIterator(Tail->next);
        }
        ConstReverseIterator crend() {
            return ConstReverseIterator(Tail->next);
        }

        [[nodiscard]] bool empty() const final {
            return Head == nullptr;
        }
        [[nodiscard]] std::size_t size() const final {
            return len;
        }

        void clear() {
            ~List();
            Head = nullptr;
            Tail = nullptr;
            len = 0;
        }

        iterator insert(ConstIterator pos, const T& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = val;
            new_node->next = pos;
            new_node->prev = pos->prev;
            pos->prev->next = new_node;
            pos->prev = new_node;
            ++len;
            return iterator(new_node);
        }
        iterator insert(ConstIterator pos, const T&& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = std::move(val);
            new_node->next = pos;
            new_node->prev = pos->prev;
            pos->prev->next = new_node;
            pos->prev = new_node;
            ++len;
            return iterator(new_node);
        }
        iterator insert(ConstIterator pos, const std::size_t cnt, const T& val) {
            if (cnt == 0) {
                return pos;
            }
            node* tmp = pos->prev;
            for (std::size_t i = 0; i < cnt; i++) {
                insert(pos, val);
            }
            len += cnt;
            return iterator(tmp->next);
        }
        template<class InputIt>
        iterator insert(ConstIterator pos, InputIt first, InputIt last) {
            if (first == last) {
                return pos;
            }
            node* tmp = pos->prev;
            InputIt* it = first;
            while (it != last) {
                insert(pos, *it);
                ++it;
                ++len;
            }
            return iterator(tmp->next);
        }
        iterator insert(ConstIterator pos, std::initializer_list<T> ilist) {
            return insert(pos, ilist.begin(), ilist.end());
        }

        iterator erase(ConstIterator pos) {
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
            node* tmp = pos->next;
            alloc.deallocate(pos, 1);
            --len;
            return iterator(tmp);
        }
        iterator erase(ConstIterator first, ConstIterator last) {
            first->prev->next = last;
            last->prev = first->prev;
            iterator it = first;
            iterator next;
            while (it != last) {
                next = ++it;
                alloc.deallocate(it, 1);
                it = next;
                --len;
            }
            return last;
        }

        void push_back(const T& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = val;
            new_node->next = nullptr;
            new_node->prev = Tail;
            Tail->next = new_node;
            Tail = new_node;
            ++len;
        }
        void push_back(T&& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = std::move(val);
            new_node->next = nullptr;
            new_node->prev = Tail;
            Tail->next = new_node;
            Tail = new_node;
            ++len;
        }
        void pop_back() {
            Tail = Tail->prev;
            alloc.deallocate(Tail->next, 1);
            Tail->next = nullptr;
            --len;
        }

        void push_front(const T& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = val;
            new_node->next = Head;
            new_node->prev = nullptr;
            Head->prev = new_node;
            Head = new_node;
            ++len;
        }
        void push_front(T&& val) {
            node* new_node = alloc.allocate(1);
            new_node->val = std::move(val);
            new_node->next = Head;
            new_node->prev = nullptr;
            Head->prev = new_node;
            Head = new_node;
            ++len;
        }
        void pop_front() {
            Head = Head->next;
            alloc.deallocate(Head->prev, 1);
            Head->prev = nullptr;
            --len;
        }

        void resize(std::size_t cnt, const T& val) {
            if (cnt == len) {
                return;
            }
            if (cnt < len) {
                erase(begin() + cnt, end());
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
                erase(begin() + cnt, end());
            }
            while (len != cnt) {
                node* new_node = alloc.allocate(1);
                new_node->next = nullptr;
                new_node->prev = Tail;
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

        bool operator==( const List<T, Allocator>& other ) {
            if (len != other.len) {
                return false;
            }
            node* l = Head, r = other.Head;
            for (std::size_t i = 0; i < len; i++) {
                if (l->val != other->val) {
                    return false;
                }
                l = l->next;
                r = r->next;
            }
            return true;
        }
        bool operator!=( const List<T, Allocator>& other ) {
            return !(this == other);
        }

        bool operator<( const List<T, Allocator>& other ) {
            node* l = Head, r = other.Head;
            for (std::size_t i = 0; i < len; i++) {
                if (l->val != other->val) {
                    return l->val < other->val;
                }
                l = l->next;
                r = r->next;
            }
            return len < other.len;
        }
        bool operator<=( const List<T, Allocator>& other ) {
            return ((this == other)  || (this < other));
        }
        bool operator>( const List<T, Allocator>& other ) {
            return !(this <= other);
        }
        bool operator>=( const List<T, Allocator>& other ) {
            return !(this < other);
        }
        std::weak_ordering operator<=>( const List<T, Allocator>& other ) {
            if (this > other) {
                return std::weak_ordering::greater;
            }
            if (this == other) {
                return std::weak_ordering::equivalent;
            }
            return std::weak_ordering::less;
        }
    };
}