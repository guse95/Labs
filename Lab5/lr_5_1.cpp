#include <iostream>
#include <utility>


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
    int len = sizeof(int) << 3;
    for (int i = 0; i < len; i = increment(i)) {
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
    binary_int() { num = 0; }
    explicit binary_int(const int val) : num(val) {}
    binary_int(const binary_int& old) { num = old.num; }
    ~binary_int () = default;

    binary_int& operator- () {
        num = add(~num, 1);
        return *this;
    }
    binary_int& operator++ () {
        num = increment(num);
        return *this;
    }
    binary_int operator++ (int) {
        binary_int tmp = *this;
        num = increment(num);
        return tmp;
    }
    binary_int& operator-- () {
        num = dif(num, 1);
        return *this;
    }
    binary_int operator-- (int) {
        binary_int tmp = *this;
        num = dif(num, 1);
        return tmp;
    }
    binary_int& operator+= (const binary_int& other) {
        num = add(num, other.num);
        return *this;
    }
    binary_int operator+ (binary_int& other) const {
        binary_int ans = binary_int{num};
        ans += other;
        return ans;
    }
    binary_int& operator-= (const binary_int& other) {
        num = dif(num, other.num);
        return *this;
    }
    binary_int operator- (binary_int& other) const {
        binary_int ans = binary_int{num};
        ans -= other;
        return ans;
    }
    binary_int& operator*= (const binary_int& other) {
        num = mult(num, other.num);
        return *this;
    }
    binary_int operator* (binary_int& other) const {
        binary_int ans = binary_int{num};
        ans *= other;
        return ans;
    }

    binary_int& operator<<= (int a) {
        num <<= a;
        return *this;
    }
    binary_int& operator>>= (int a) {
        num >>= a;
        return *this;
    }

    binary_int operator>> (int a) const {
        binary_int ans = binary_int{num};
        ans >>= a;
        return ans;
    }
    binary_int operator<< (int a) const {
        binary_int ans = binary_int{num};
        ans <<= a;
        return ans;
    }

    binary_int& operator= (const binary_int& other) {
        num = other.num;
        return *this;
    }
    friend std::ostream& operator << (std::ostream &out, const binary_int& a);
    friend std::pair<binary_int, binary_int> split(const binary_int& obg);


private:
    int num;
};


int PowOf2(int base) {
    int p = 0;
    if (base < 0) {
        return sizeof(int) << 3;
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



std::pair<binary_int, binary_int> split(const binary_int& obg) {
    binary_int low(0);
    binary_int top(0);
    int len = PowOf2(obg.num);
    int half = (len & 1) ? add(len>>1, 1): len>>1;

    for (int i = 0; i < half; i = increment(i)) {
        low += binary_int{((obg.num>>i) & 1) << i};
    }
    for (int i = half; i <= len; i = increment(i)) {
        top += binary_int{((obg.num>>i) & 1) << i};
    }
    return {top, low};
}

int main() {
    binary_int a(13);
    binary_int b(52);
//    std::cout << b << '\n';
    binary_int c;
    c = b - a;
    std::cout << b-- << ' ' << a << " = " << c << '\n';
    std::cout << b;
//    std::pair<binary_int, binary_int> ama = split(b);

//    std::cout << ama.first << ' ' << ama.second;
//    std::cout << c << ' ' << a;
}