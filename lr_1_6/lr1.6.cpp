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
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_EPS_OUT_OF_RANGE
};

typedef double(*callback)(double);

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
    if (len > 8) {
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
        case ERROR_EPS_OUT_OF_RANGE:
            printf("Epsilon out of range.\n");
            break;
        case ERROR_ZERO_VAL:
            printf("Epsilon is zero.\n");
            break;
        default:
            break;
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

ret_type_t Integral(const double eps, callback IntegrFunc) {
    int n = 2;
    double cur_summ = 0;
    double prev_summ;

    do {
        prev_summ = cur_summ;
        cur_summ = 0;
        for (int i = 1; i < n; ++i) {
            cur_summ = cur_summ + IntegrFunc(i / (double)n);
        }
        cur_summ = cur_summ / (double)n;
        ++n;
    } while (fabs(cur_summ - prev_summ) > eps);
//    if (isinf(summ) || isnan(summ)) {
//        return ERROR_ARG_IS_TOO_BIG;
//    }
    printf("%f\n", cur_summ);
    return SUCCSESS;
}

double Afunc(const double x) {
    double res;
    res = log(1 + x) / x;
    return  res;
}

double Bfunc(const double x) {
    double res;
    res = exp(-x * x / 2.0);
    return res;
}

double Cfunc(const double x) {
    double res;
    res = log(1 / (1 - x));
    return res;
}

double Dfunc(const double x) {
    double res;
    res = pow(x, x);
    return res;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }

    if (ret_type_t code = is_number(argv[1])) {
        HandlingError(code);
        return code;
    }
    double eps = atof(argv[1]);

    if (eps > 0.2) {
        HandlingError(ERROR_EPS_OUT_OF_RANGE);
        return ERROR_EPS_OUT_OF_RANGE;
    }
    callback funcs[] = {&Afunc, &Bfunc, &Cfunc, &Dfunc};

    for (callback func : funcs) {
        HandlingError(Integral(eps, func));
    }

    return SUCCSESS;
}