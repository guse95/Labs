#include <iostream>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

enum ret_type_t {
    SUCCSESS,
    ERROR_NO_VALUE,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_NUMBER,
    ERROR_NUMBER_OUT_OF_RANGE
};

typedef ret_type_t(*callback)(char*);


ret_type_t is_number(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') s++;

    int len = 0;
    while (isdigit(*s)) {
        s++;
        if (len++ > 10) return ERROR_TOO_LONG_NUMBER;
    }

    if (*s == '\0') return SUCCSESS;
    return ERROR_NOT_NUMBER;
}

int atoi(const char* s) {
    while (*s == ' ') s++;
    int flag = 1;
    if (*s == '-') {
        flag = -1;
        s++;
    }
    int res = 0;
    while (*s != '\0') {
        res = (res * 10) + (*s - '0');
        s++;
    }
    res *= flag;
    return res;
}

long long comb(long long up, long long down) {
    long long res = 1;
    if (up < (down - up)) {
        up = down - up;
    }
    for (long long i = up + 1; i < down + 1; ++i) {
        res *= i;
    }
    for (long long i = 2; i < (down - up) + 1; i += 1) {
        res /= i;
    }
    printf("%lld  %lld  %lld\n", down, up, res);
    return res;
}

int main(){
    char str[] = "12345678987654";
    printf("%d", pow(2, 31));

}
