#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>


enum ret_type_t {
    SUCCSESS,
    ERROR_NO_VALUE,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NUMBER_OUT_OF_RANGE,
    ERROR_WRONG_NUMBER_OF_ARGS
};

typedef ret_type_t(*callback)(int, char*[]);

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_NO_VALUE:
            printf("No number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value isn`t a number.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_NUMBER_OUT_OF_RANGE:
            printf("Number out of range.\n");
            break;
        case ERROR_WRONG_NUMBER_OF_ARGS:
            printf("Wrong number of arguments.\n");
            break;
        default:
            break;
    }
}

int Atoi(const char* s) {
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

ret_type_t is_int(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') s++;

    int len = 0;
    while (isdigit(*s)) {
        s++;
        if (len++ > 9) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCSESS;
    return ERROR_NOT_NUMBER;
}

ret_type_t is_double(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') s++;

    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && point_was)) {
        if (*s == '.') point_was += 1;
        s++;
        if (len++ > 10) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCSESS;
    return ERROR_NOT_NUMBER;
}

int findFlag(char* curarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i)
    {
        if (!(strcmp(curarg, flags[i])))
        {
            return i;
        }
    }
    return -1;
}

void ResOfEquation(double *arr) {

}

double permute(double *arr, int l, int r) {
    if (l == r) {
        ResOfEquation(arr);
        for (int i = 0; i <= r; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    } else {
        for (int i = l; i <= r; i++) {
            swap(&arr[l], &arr[i]);
            permute(arr, l + 1, r);
            swap(&arr[l], &arr[i]);
        }
    }
}

ret_type_t funcForQ(int cnt, char* input[]) {
    if (cnt != 6) {
        return ERROR_WRONG_NUMBER_OF_ARGS;
    }
    ret_type_t code;
    for (int i = 2; i < cnt; ++i) {
        if (!(code = is_double(input[i]))) {
            HandlingError(code);
            return code;
        }
    }
    double eps = atof(input[2]);
    double kef[3];
    for (int i = 3; i < cnt; ++i) {
        kef[i - 3] = atof(input[i]);
    }

}

int main(int argc, char* argv[]) {

    const char* flags[] = { "-q", "/q", "-m", "/m", "-t", "/t" };
    callback cbs[] = { &funcForQ, &funcForM, &funcForT };

    for (int i = 1; i < argc; ++i)
    {
        int ret = findFlag(argv[i], flags, sizeof(flags) / sizeof(char*));
        if (ret == -1)
        {
            printf("THIS FLAG DOES NOT EXIST %s\n", argv[i]);
            return -1;
        }
        else
        {
            int findCbsInt = ret / 2;
            callback find = cbs[findCbsInt];
            if (int code = find(argc, argv)) {
                HandlingError(code);
                return -1;
            }
            i++;
        }

    }

    return 0;
}