#include <stdio.h>
#include <math.h>
#include <ctype.h>

enum ret_type_t {
    SUCCSESS,
    ERROR_NO_VALUE,
    ERROR_ZERO_VAL,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NUMBER_OUT_OF_RANGE,
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_ARG_IS_TOO_BIG
};

typedef double(*callback)(double, int);

ret_type_t is_number(const char* s) {
    int flag = 0;
    if (*s == '\0') return ERROR_NO_VALUE;
    while (*s == ' ') s++;
    int len = 0;
    while (isdigit(*s)) {
        if (*s != '0') flag = 1;
        len++;
        s++;
    }
    if (*s == '.') s++;
    while (isdigit(*s)) {
        if (*s != '0') flag = 1;
        s++;
        len++;
    }
    if (!flag) {
        return ERROR_ZERO_VAL;
    }
    if (len > 10) {
        return ERROR_TOO_LONG_STR;
    }
    if (*s == '\0') return SUCCSESS;
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
        case ERROR_ARG_IS_TOO_BIG:
            printf("The function diverges.\n");
            break;
        case ERROR_ZERO_VAL:
            printf("Value is zero.\n");
            break;
        default:
            break;
    }
}

double faq(const int n) {
    double res = 0;
    if (n == 0) {
        return 1.0;
    } else {
        res = n * faq(n - 1);
        return res;
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

double Pow(const double x, const int n) {
    double res = 1;

    if(n == 0) {
        res = 1.0;
        return res;
    }

    for(int _ = 0; _ < abs(n); ++_) {
        res *= x;
    }
    if (n < 0) {
        res = 1.0 / res;
    }
    return res;
}

ret_type_t Row(const double eps, callback RowFunc, const double x, const int n) {
//    int m = n;
//    double PrevAns = RowFunc(x, m);
//    m += 1;
//    double CurAns = PrevAns + RowFunc(x, m);
//    while (fabs(PrevAns - CurAns) > eps) {
//        PrevAns = CurAns;
//        m += 1;
//        CurAns += RowFunc(x, m);
//    }
//    printf("%fl\n", CurAns);
//    if (isinf(summ)) {
//        printf("mi ymerli");
//        return ERROR_ARG_IS_TOO_BIG;
//    }
//    printf("%f\n", summ);
//    return SUCCSESS;

    int m = n;
    double summ = RowFunc(x, m);
    double cur_delta;

    do {
        m += 1;
        cur_delta = RowFunc(x, m);
        summ = summ + cur_delta;

    } while (fabs(cur_delta) > eps);
    if (isinf(summ) || isnan(summ)) {
        return ERROR_ARG_IS_TOO_BIG;
    }
    printf("%f\n", summ);
    return SUCCSESS;
}

double Afunc(const double x, const int n) {
    double res;
    res = Pow(x, n) / faq(n);
//    printf("%d  %f  %f  %f\n", n, Pow(x, n), faq(n), res);
    return  res;
}

double Bfunc(const double x, const int n) {
    double res;
    res = Pow(x, 2 * n) / faq(2 * n);
    if (n % 2 == 1) {
        res = -res;
    }
    return res;
}

double Cfunc(const double x, const int n) {
    double res;
    res = Pow(3, 3 * n) / faq(3 * n) * Pow(faq(n), 3) * Pow(x, 2 * n);
    return res;
}

double Dfunc(const double x, const int n) {
    double res;
    res = doublefaq(2 * n - 1) / doublefaq(2 * n) * Pow(x, 2 * n);
//    printf("%f  %f  %f\n", doublefaq(2 * n - 1), Pow(x, 2 * n), doublefaq(2 * n));
    if (n % 2 == 1) {
        res = -res;
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }

    if (ret_type_t code = is_number(argv[1])) {
        HandlingError(code);
        return code;
    }
    double eps = atof(argv[1]);

    if (ret_type_t code = is_number(argv[2])) {
        HandlingError(code);
        return code;
    }
    double x = atof(argv[2]);

    HandlingError(Row(eps, Afunc, x, 0));
    HandlingError(Row(eps, Bfunc, x, 0));
    HandlingError(Row(eps, Cfunc, x, 0));
    HandlingError(Row(eps, Dfunc, x, 1));
    return SUCCSESS;
}