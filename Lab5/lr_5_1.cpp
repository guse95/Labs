#include <iostream>
#include <string>


int increment(int i) {
    int mask = 1;
    while ((i & mask) != 0) {
        i = i ^ mask;
        mask = mask<<1;
    }
    i = i ^ mask;
    return i;
}

int add(int a, int b) {
    int mind = 0, ans = 0;
    for (int i = 0; i < 32; i = increment(i)) {
        int dig1 = ((a & (1 << (i))) >> (i));
        int dig2 = ((b & (1 << (i))) >> (i));

        int cur = (dig1 ^ dig2 ^ mind);
        mind = (mind == 0) ? (dig1 & dig2) : (dig1 | dig2);
        ans = ans | (cur << (i));
    }
    return ans;
}

int dif(int a, int b) {
    return add(a, add(~b, 1));
}

int mult(int a, int b) {
    int ans = 0;
    if (a < b) {
        int tmp = a;
        a = b;
        b = tmp;
    }
    int i = 0;
    while (b != 0) {
        ans = (b & 1) ? add(ans, (a << (i))) : ans;
        b = b >> 1;
        i = increment(i);
    }


    return ans;
}

class binary_int {

public:


    binary_int() { this->num = 0; }
    explicit binary_int(int val) : num(val) {}
    binary_int(binary_int& old) { num = old.num; }
    ~binary_int () { num = 0; }

    binary_int& operator- () {
        num = add(~num, 1);
        return *this;
    }
    binary_int& operator++ () {
        num = increment(num);
        return *this;
    }
    binary_int& operator-- () {
        num = dif(num, 1);
        return *this;
    }
    binary_int& operator+= (const binary_int& a) {
        num = add(num, a.num);
        return *this;
    }
    binary_int operator+ (binary_int& a) {
        int ans;
        ans = add(num, a.num);
        return binary_int{ans};
    }
    binary_int& operator-= (const binary_int& a) {
        num = dif(num, a.num);
        return *this;
    }
    binary_int operator- (binary_int& a) {
        int ans;
        ans = dif(num, a.num);
        return binary_int{ans};
    }
    binary_int& operator*= (const binary_int& a) {
        num = mult(num, a.num);
        return *this;
    }
    binary_int operator* (binary_int& a) {
        int ans;
        ans = mult(num, a.num);
        return binary_int{ans};
    }

    binary_int& operator<<= (int a) {
        num <<= a;
        return *this;
    }
    binary_int& operator>>= (int a) {
        num >>= a;
        return *this;
    }

    binary_int operator>> (int a) {
        int ans;
        ans = num >> a;
        return binary_int{ans};
    }
    binary_int operator<< (int a) {
        int ans;
        ans = num << a;
        return binary_int{ans};
    }

    binary_int& operator= (const binary_int& a) {
        num = a.num;
        return *this;
    }
    friend std::ostream& operator << (std::ostream &out, const binary_int& a);
    friend void split(binary_int obg, binary_int* ans1, binary_int* ans2);


private:
    int num;
};


int PowOf2(int base) {
    int p = 0;
    if (base < 0) {
        return 32;
    }
    if (base == 0) return 1;
    while (base) {
//        std::cout << base << '\n';
        base >>= 1;

        p = increment(p);
    }
    return p;
}

std::ostream& operator << (std::ostream &out, const binary_int& a) {
    int len = dif(PowOf2(a.num), 1);
//    std::cout << len << '\n';
    for (int i = len; i >= 0; i = dif(i, 1)) {
        out << ((a.num>>i) & 1);
    }
    return out;
}



void split(binary_int obg, binary_int* low, binary_int* top) {
    *low = binary_int{0};
    *top = binary_int{0};
    int len = PowOf2(obg.num);
    int half = (len & 1) ? add(len>>1, 1): len>>1;

    for (int i = 0; i < half; i = increment(i)) {
        *low += binary_int{((obg.num>>i) & 1) << i};
    }
    for (int i = half; i <= len; i = increment(i)) {
        *top += binary_int{((obg.num>>i) & 1) << i};
    }
//    std::cout << *top << ' ' << *low << '\n';
}

int main() {
    binary_int a(13);
    binary_int b(-1234);
//    std::cout << b << '\n';
    binary_int c;
//    c = b >> 1;
    split(b, &a, &c);
    std::cout << c << ' ' << a;

}