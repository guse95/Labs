#include <iostream>

using ll = long long;

int main(){
    ll l = 0;
    ll r = 52;
    ll tmp = 24;
    ll other = 8;
    ll m;
    while (l + 1 < r) {
        m = (l + r) / 2;
        ll mult = (other * m);
        if (tmp >= mult) {
            l = m;
        } else {
            r = m;
        }
    }
    std::cout << l << std::endl;
    std::cout << m << std::endl;
    std::cout << r << std::endl;
}
