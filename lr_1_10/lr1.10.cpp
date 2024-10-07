#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

enum ret_type_t {
    SUCCSESS,
    ERROR,
    ERROR_NO_VALUE,
    ERROR_TOO_LONG_STR,
    ERROR_NOT_NUMBER,
    ERROR_NEGATIVE_VALUE,
    ERROR_NUMBER_OUT_OF_RANGE
};

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_NO_VALUE:
            printf("No number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value isn`t a number in the right number system.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_NUMBER_OUT_OF_RANGE:
            printf("Number out of range.\n");
            break;
        default:
            break;
    }
}

ret_type_t isNumberInSsWithBase(const int base, const char* s) {
    if (*s == '\0') {
        return ERROR_NO_VALUE;
    }
    while (*s == ' ') {
        s++;
    }
//    if (*s == '-') {
//        return ERROR_NEGATIVE_VALUE;
//    }
    int len = 0;
    int maxLen = 0, intLim = 100000000;
    while (intLim > 0) {
        intLim /= base;
        maxLen++;
    }
    printf("%d\n", maxLen);
    while ((isdigit(*s) && (*s - '0' < base)) || (isalpha(*s) && (toupper(*s) < 'A' + base - 10))) {
        s++;
        if (len++ > maxLen) {
            return ERROR_TOO_LONG_STR;
        }
    }

    if (*s == '\0') {
        return SUCCSESS;
    }
    return ERROR_NOT_NUMBER;
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

void FromDesToSist(const int base, int x){
    char buf[BUFSIZ], *pbuf = buf + BUFSIZ - 1, r;
    *pbuf-- = 0;
    while (x) {
        *pbuf-- = ((r = x % base) > 9) ? r - 10 + 'A' : r + '0';
        x /= base;
    }
    while (*++pbuf) {
        printf("%c", *pbuf);
    }
    printf("\n");
}

int FromSistToDes(const int base, char str[]){
    int res = 0;
    char *pstr = str - 1;
    while (*++pstr) {
        res = res * base + (isalpha(*pstr) ? toupper(*pstr) - 'A' + 10 : *pstr - '0');
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Not enough arguments.");
        return ERROR;
    }
    if (ret_type_t code = isNumberInSsWithBase(10, argv[1])) {
        HandlingError(code);
        return code;
    }
    int base = Atoi(argv[1]);
    int MaxVal = 0, CurVal;
    for (int i = 2; i < argc; ++i) {
        if (ret_type_t code = isNumberInSsWithBase(base, argv[i])) {
            HandlingError(code);
            return code;
        }
//        printf("%d\n", FromSistToDes(base, argv[i]));
        if ((CurVal = FromSistToDes(base, argv[i])) > MaxVal) {

            MaxVal = CurVal;
        }
    }
    for (int i = 9; i <= 36; i += 9) {
        FromDesToSist(i, MaxVal);
    }
    return SUCCSESS;
}