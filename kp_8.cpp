#include <iostream>

using namespace std;



class Node {
public:
    int data;

private:
    Node* next;

public:
    Node() :data(0), next(nullptr) {};
    Node(int d) : data(d), next(nullptr) {};
    Node(Node& nod) : data(nod.data), next(nullptr) {};
    ~Node() { data = 0; next = nullptr; }
    friend class List;

    void printNode() const {
        cout << data << '\n';
    }
};

Node* EOL = new Node(1e9 + 7);

class List {
    Node* head;
    int len{};

public:
    List() : head(EOL) {};
    List(int l, int val) : head(nullptr), len(l) {
        head = new Node(val);
        Node* a = new Node(val);
        head->next = a;
        for (int i = 0; i < l - 1; ++i) {
            a->next = new Node(val);
            a = a->next;
        }
        a->next = EOL;
    };
    List(List& lis) {
        // Node* root = lis.head;
        // head = new Node(root->data);
        // root = root->next;
        // Node * a = new Node(root->data);
        // head -> next = a;
        // while (root != nullptr) {
        //     a -> next = new Node(root->data);
        //     root = root->next;
        //     a = a -> next;
    };

    ~List() { len = 0, head = EOL; }

    int getIndex(int v, int i = 0, int j = 10) {
        Node* a = head;
        for (; i <= len - 1; ++i) {
            if (a->data == v) {
                return i;
            }
            a = a->next;
        }
        throw -1;
    }

    int& operator[](int i) {
        Node* a = head;
        if (len <= i) {
            throw - 1;
        }
        while (i > 0) {
            a = a->next;
            --i;
        }
        return a->data;
    }

    int length() {
        return len;
    }

    void appEnd(int vak) {
        Node* a = head;
        while (a->next != EOL) {
            a = a->next;
        }
        a->next = new Node(vak);
        a->next->next = EOL;
        len += 1;
    }

    bool isEmpty() {
        return head == EOL;
    }

    void printList() {
        Node* a = head;
        for (int i = 0; i < len; ++i) {
            a->printNode();
            a = a->next;
        }
        cout << '\n';
    }

    void deleteList() {
        head = EOL;
        delete[] & len;
    }

    Node* findNode(int ind) {
        // int ind = getIndex(v);
        if (len <= ind) return EOL;
        Node* a = head;
        while (ind > 0) {
            a = a->next;
            --ind;
        }
        return a;
    }

    Node* findPrev(int v) {
        return findNode(v - 1);
    }

    Node* findNext(int v) {
        return findNode(v + 1);
    }

    void insertNext(int ind, int val) {
        if (len <= ind) throw -1;
        Node* cur = findNode(ind);
        Node* cur_next = cur->next;
        Node* a = new Node(val);
        cur->next = a;
        a->next = cur_next;
        ++len;
    }

    void swap_sec_minussec() {
        if (len < 2) throw -1;
        Node* sec = findNode(1);
        Node* prev2 = findPrev(len - 2);
        Node* minussec = prev2->next;
        Node* next1 = sec->next;
        Node* next2 = minussec->next;
        head->next = minussec;
        minussec->next = next1;
        prev2->next = sec;
        sec->next = next2;
    }
};

int main() {
    List mama1(10, 4);
    mama1.insertNext(0, 11);
    for (int i = 0; i < 11; ++i) {
        cout << mama1[i] << ' ';
    }

    cout << "\n\n\n";
    mama1.swap_sec_minussec();
    
    for (int i = 0; i < 11; ++i) {
        cout << mama1[i] << ' ';
    }
}
