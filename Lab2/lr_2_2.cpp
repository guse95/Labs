#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>
#include <math.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_ZERO_VAL,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
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
            printf("Value is not a number.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_WRONG_NUMBER_OF_ARGS:
            printf("Wrong number of arguments.\n");
            break;
        case ERROR_ZERO_VAL:
            printf("Value is zero.\n");
            break;
        default:
            printf("ABORTED!!!");
            break;
    }
}

double avg_geom(int argc, ...) {
    va_list numb;
    va_start(numb, argc);
    double total = 1;
    for (int i = 0; i < argc; ++i) {
        total *= pow(va_arg(numb, double), 1.0/argc);
    }
    return total;
}

double fast_pow(const double numb, int power) {
    if (power < 2) {
        if (power) {
            return numb;
        }
        return 1;
    }
    double res = fast_pow(numb, (power/2));
    res = res * res;
    if (power % 2 == 1) {
        res = res * numb;
    }
    return res;
}

int main() {
    double ans = avg_geom(3, 2.0, 4.0, 3.0);
    double res = fast_pow(3.14, 6);
    printf("%.10lf", res);
}
