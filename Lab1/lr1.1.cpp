#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
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
        if (len++ > 9) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

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

ret_type_t funcForH(char* argv) {
    if (ret_type_t code = is_number(argv)) return code;
    const int x = abs(Atoi(argv));
    int ans = x;
    if (x > 100)
    {
        printf("There are no such numbers.\n");
        return SUCCESS;
    }
    while (ans <= 100)
    {
        printf("%d\n", ans);
        ans += x;
    }
    return SUCCESS;
}

ret_type_t funcForP(char* argv) {
    if (ret_type_t code = is_number(argv)) {
        return code;
    }
    const int x = Atoi(argv);
    if (x <= 1)
    {
        printf("X isn`t composite or simple.\n");
        return SUCCESS;
    }
    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0) {
            printf("X is composite.\n");
            return SUCCESS;
        }
    }
    printf("X is simple.\n");
    return SUCCESS;
}

ret_type_t funcForS(char* argv) {
    if (ret_type_t code = is_number(argv)) return code;
    int x = Atoi(argv);
    if (x < 1) return ERROR_NEGATIVE_VALUE;
    int base = 16, res = 0;
    char buf[BUFSIZ], *pb = buf + BUFSIZ - 1, r;
    *pb-- = 0;
    while (x) {
        *pb-- = ((r = x % base) > 9) ? r - 10 + 'A' : r + '0';
        x /= base;
    }
    while (*++pb)
        printf("%c ", *pb);
    printf("\n");

    return SUCCESS;
}

ret_type_t funcForE(char* argv) {
    if (ret_type_t code = is_number(argv)) return code;
    const int x = Atoi(argv);
    if (x < 1 || x > 10) return ERROR_NUMBER_OUT_OF_RANGE;
    for (int base = 1; base <= 10; base++)
    {
        long long ans = 1;
        for (int _ = 1; _ <= x; _++)
        {
            ans *= base;
            printf("%-11lld", ans);
        }
        printf("\n");
    }
    return SUCCESS;
}

ret_type_t funcForA(char* argv) {
    if (ret_type_t code = is_number(argv)) return code;
    const long long x = Atoi(argv);
    if (x < 1) return ERROR_NEGATIVE_VALUE;
    long long summ = (x % 2 == 0) ? (x / 2) * (x + 1): ((x + 1) / 2) * x;
    printf("%lld\n", summ);
    return SUCCESS;
}

ret_type_t funcForF(char* argv) {
    if (ret_type_t code = is_number(argv)) return code;
    const int x = Atoi(argv);
    if (x < 1) return ERROR_NEGATIVE_VALUE;
    if (x > 34)
    {
        printf("The factorial exceeds 2^128.\n");
        return ERROR_NUMBER_OUT_OF_RANGE;
    }
    unsigned long long faq = 1;
    for (int i = 2; i <= x; i++)
    {
        faq *= i;
    }
    printf("%lld\n", faq);
    return SUCCESS;
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

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments.\n");
        return -1;
    }

    const char* flags[] = { "-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f" };
    callback cbs[] = { &funcForH, &funcForP, &funcForS, &funcForE, &funcForA, &funcForF };
    
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
            if (int code = find(argv[i + 1])) {
                HandlingError(code);
                return -1;
            }
            i++;
        }

    }

    return 0;
}