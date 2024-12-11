#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>

enum {
    SUCCESS,
    WRONG_NUMBER_OF_ARGS,
    WRONG_BASE,
    NOT_A_NUM,
    WRONG_NUMBER_SYSTEM,
    NUM_TOO_LARGE,
    IS_EMPTY,
    MEMORY_ALLOCATION_ERROR,
    IS_BULGING,
    IS_NOT_BULGING,
    IS_KAPREKAR,
    IS_NOT_KAPREKAR
};

void HandlingError(int code) {
    switch (code) {
        case SUCCESS: {
            printf("SUCCESS\n");
            break;
        }
        case IS_BULGING: {
            printf("IS_BULGING\n");
            break;
        }
        case IS_NOT_BULGING: {
            printf("IS_NOT_BULGING\n");
            break;
        }
        case WRONG_NUMBER_OF_ARGS: {
            printf("WRONG_NUMBER_OF_ARGS\n");
            break;
        }
        case MEMORY_ALLOCATION_ERROR: {
            printf("MEMORY_ALLOCATION_ERROR\n");
            break;
        }
        case WRONG_BASE: {
            printf("WRONG_BASE\n");
            break;
        }
        case WRONG_NUMBER_SYSTEM: {
            printf("WRONG_NUMBER_SYSTEM\n");
            break;
        }
        case NUM_TOO_LARGE: {
            printf("NUM_TOO_LARGE\n");
            break;
        }
        case IS_EMPTY: {
            printf("IS_EMPTY\n");
            break;
        }
        case NOT_A_NUM: {
            printf("NOT_A_NUM or WRONG_NUMBER_SYSTEM\n");
            break;
        }
        default: {
            printf("ABORTED\n");
            break;
        }
    }
}

int isNumber(const char* argv, int base) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isdigit(*argv) && (!islower(*argv))) {
            return NOT_A_NUM;
        } else if (isdigit(*argv)) {
            if (*argv - '0' >= base) {
                return WRONG_NUMBER_SYSTEM;
            }
            argv++;
            cnt++;
        } else if (isalpha(*argv)) {
            if ((*argv - 'a') >= (base - 10)) {
                return WRONG_NUMBER_SYSTEM;
            }
            argv++;
            cnt++;
        } else {
            argv++;
            cnt++;
        }
    }
    if (cnt > 9) {
        return NUM_TOO_LARGE;
    }
    if (cnt == 0) {
        return IS_EMPTY;
    }
    return SUCCESS;
}

double Pow(double number, int power) {
    if (power < 2) {
        if (power) {
            return number;
        }
        return 1;
    }
    double res = Pow(number, (power/2));
    res = res * res;
    if (power % 2 == 1) {
        res = res * number;
    }
    return res;
}

long long FromSistToDes (const int base, char *str) {
    long long res = 0, negFlag = 0;
    char *p = str;

    if (*p == '-') {
        negFlag = 1;
        p++;
    }

    while (*p) {
        if (isalpha(*p)) {
            res = res * base + *p++ - 'a' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
    }
    if (negFlag) {
        res = -res;
    }
    return res;
}

char * FromDestoSist(long long num, int base) {
    char buf[BUFSIZ], *p = buf + BUFSIZ - 1;
    long long r;
    int len = 0;
    *p-- = '\0';

    while (num) {
        r = num % base;
        if (r > 9) {
            *p = r - 10 + 'a';
        } else {
            *p = r + '0';
        }
        p--;
        num /= base;
        len++;
    }
    char * res = (char*)malloc(len + 1);
    if (res == NULL) {
        printf("malloc failed\n");
        return NULL;
    }
    for (int i = 0; i < len; i++) {
        res[i] = *(p+1+i);
    }
    res[len] = '\0';
    return res;
}

struct Point {
    double x;
    double y;
};

double Determinant(struct Point p1, struct Point p2, struct Point p3) {
    double Ux = p2.x - p1.x;
    double Uy = p2.y - p1.y;
    double Vx = p3.x - p2.x;
    double Vy = p3.y - p2.y;
    return (Ux * Vy) - (Uy * Vx);
}

int IsBulging(int cnt, ...) {
    if (cnt < 6 || cnt % 2 != 0) {
        return WRONG_NUMBER_OF_ARGS;
    }
    va_list arg;
    va_start(arg, cnt);

    struct Point * array = (struct Point*)malloc(sizeof(struct Point) * cnt);
    if (array == NULL) {
        va_end(arg);
        return MEMORY_ALLOCATION_ERROR;
    }

    for (int i = 0; i < cnt; i++) {
        double x = va_arg(arg, double);
        double y = va_arg(arg, double);
        array[i].x = x;
        array[i].y = y;
    }

    int n = cnt / 2;
    double sign = 0;
    for (int i = 0; i < n; i++) {
        double opr = Determinant(array[i], array[(i + 1) % n], array[(i + 2) % n]);

        if (opr != 0) {
            if (sign == 0) {
                sign = (opr > 0) ? 1 : -1;
            } else if ( (opr > 0 && sign < 0) || (opr < 0 && sign > 0) ) {
                free(array);
                return IS_NOT_BULGING;
            }
        }
    }
    va_end(arg);
    free(array);
    return IS_BULGING;
}

double ExpressionInPoint(double x, int power, ...) {
    va_list arg;
    va_start(arg, power);

    double res = 0;
    for (int i = power; i >= 0; i--) {
        int koef = va_arg(arg, int);
        double slag = Pow(x, i) * koef;
        res += slag;
    }
    va_end(arg);
    return res;
}

int strLen(const char *str) {
    const char * p = str;
    int res = 0;
    while (*p != '\0') {
        res++;
        p++;
    }
    return res;
}

long long PartFromSistToDes (const int base, const char *str, int l, int r) {
    long long res = 0;
    int len = r - l + 1;
    const char *p = str;

    p += l;

    while (*p && len > 0) {
        if (isalpha(*p)) {
            res = res * base + *p++ - 'a' + 10;
        }
        else {
            res = res * base + *p++ - '0';
        }
        len--;
    }
    return res;
}

int IsKaprekar(char * num, int base) {
    long long number = FromSistToDes(base, num);
    long long square = number * number;

    char * sq_in_sist = FromDestoSist(square, base);
    int len = strLen(sq_in_sist);
    for (int split = 0; split < len; split++) {
        long long l = PartFromSistToDes(base, sq_in_sist, 0, split);
        long long r = PartFromSistToDes(base, sq_in_sist, split + 1, len - 1);
        if (l + r == number) {
            free(sq_in_sist);
            return IS_KAPREKAR;
        }
    }
    free(sq_in_sist);
    return IS_NOT_KAPREKAR;
}

int FindKaprekar(int base, int count, ...) {
    if (base < 2 || base > 36) {
        return WRONG_BASE;
    }
    if (count < 1) {
        return WRONG_NUMBER_OF_ARGS;
    }

    va_list arg;
    va_start(arg, count);

    for (int i = 0; i < count; i++) {
        char *num = va_arg(arg, char *);
        int code = isNumber(num, base);
        if (code) {
            va_end(arg);
            return code;
        }
        code = IsKaprekar(num, base);
        if (code == IS_KAPREKAR) {
            printf("Is kaprekar %s.\n", num);
        } else {
            printf("Is not kaprekar %s.\n", num);
        }
    }

    return SUCCESS;
}

int main() {
    int ret = IsBulging(12, 1.0, 0.0, 3.0, 2.0, 6.0, 3.0, 7.0, 3.0, 9.0, 1.0, 5.0, 0.0);
    HandlingError(ret);

    printf("%f\n", ExpressionInPoint(0.5, 4, 19, 5, 4, -10, 0));

    ret = FindKaprekar(16, 4, "ff", "2", "8d874", "9d036");
    if (ret != SUCCESS) {
        HandlingError(ret);
    }
    return 0;
}