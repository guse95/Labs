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

void swap(double *a, double *b) {
    double buf = *a;
    *a = *b;
    *b = buf;
}

double permute(double *arr, int l, int r) {
    if (l == r) {

    } else {
        for (int i = l; i <= r; i++) {
            swap(&arr[l], &arr[i]);
            permute(arr, l + 1, r);
            swap(&arr[l], &arr[i]);
        }
    }
}

double doublefaq(const int n) {
    double res = 0;
    if (n == 0 || n == 1) {
        return 1.0;
    } else {
        res = n * doublefaq(n - 2);
        return res;
    }
}

int main(){
    double a = 2, b = 10;
    double arr[] = {1, 2, 3};
    permute(arr, 0, 2);
    for (int i = 0; i <= 2; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

}
