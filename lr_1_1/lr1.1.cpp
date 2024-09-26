#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCSESS,
    ERROR
};

typedef ret_type_t(*callback)(char*);


int is_number(const char* s) {
    if (*s == '\0') return ERROR;

    while (*s == ' ') s++;
    
    if (*s == '-') s++;

    while (isdigit(*s)) s++;

    if (*s == '\0') return SUCCSESS;
    return ERROR;
}

ret_type_t funcForH(char* argv) {
    if (is_number(argv)) return ERROR;
    const int x = abs(atoi(argv));
    int ans = x;
    if (x > 100)
    {
        printf("There are no such numbers.\n");
        return SUCCSESS;
    }
    while (ans <= 100)
    {
        printf("%d\n", ans);
        ans += x;
    }
    return SUCCSESS;
}

ret_type_t funcForP(char* argv) {
    if (is_number(argv)) return ERROR;
    const int x = atoi(argv);
    if (x <= 1)
    {
        printf("X isn`t composite or simple.\n");
        return SUCCSESS;
    }
    for (int i = 2; i * i <= x; i++)
    {
        if (x % i == 0) {
            printf("X is composite.\n");
            return SUCCSESS;
        }
    }
    printf("X is simple.\n");
    return SUCCSESS;
}

ret_type_t funcForS(char* argv) {
    if (is_number(argv)) return ERROR;
    int x = atoi(argv);
    if (x < 1) return ERROR;
    char* res = (char*)malloc(sizeof(x) / sizeof(int));
    memset(res, '\0', sizeof(x) / sizeof(int));
    int len = 0;
    while (x > 0)
    {
        res[len] =  x % 16;
        x /= 16;
        ++len;
    }
    --len;
    while (len >= 0) {
        printf("%X ", res[len]);
        --len;
    }
    printf("\n");
    free(res);
    return SUCCSESS;
}

ret_type_t funcForE(char* argv) {
    if (is_number(argv)) return ERROR;
    const int x = atoi(argv);
    if (x < 1 || x > 10) return ERROR;
    for (int base = 1; base <= 10; base++)
    {
        int ans = 1;
        for (int _ = 1; _ <= x; _++)
        {
            ans *= base;
            printf("%-10d", ans);
        }
        printf("\n");
    }
    return SUCCSESS;
}

ret_type_t funcForA(char* argv) {
    if (is_number(argv)) return ERROR;
    const int x = atoi(argv);
    if (x < 1) return ERROR;
    int summ = (x + 1) * x / 2;
    printf("%d\n", summ);
    return SUCCSESS;
}

ret_type_t funcForF(char* argv) {
    if (is_number(argv)) return ERROR;
    const int x = atoi(argv);
    if (x < 1) return ERROR;
    if (x > 34)
    {
        printf("The factorial exceeds 2^128.\n");
        return ERROR;
    }
    unsigned long long faq = 1;
    for (int i = 2; i <= x; i++)
    {
        faq *= i;
    }
    printf("%lld\n", faq);
    return SUCCSESS;
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
    if (argc < 3 || argc % 2 == 0) return -1;

    const char* flags[] = { "-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f" };
    callback cbs[] = { &funcForH, &funcForP, &funcForS, &funcForE, &funcForA, &funcForF };
    
    for (int i = 1; i < argc; ++i)
    {
        int ret = findFlag(argv[i], flags, sizeof(flags) / sizeof(char*));
        if (ret == -1)
        {
            printf("THIS FLAG DOES NOT EXIST %s", argv[i]);
            return -1;
        }
        else
        {
            int findCbsInt = ret / 2;
            callback find = cbs[findCbsInt];
            if (find(argv[i + 1])) return -1; /*Надо проверить возвращаемое значение, корректно ли писать i + 1*/
            i++;
        }

    }

    return 0;
}