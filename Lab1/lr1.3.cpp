#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_ZERO_VAL,
    ERROR_ZERO_SIDE,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NUMBER_OUT_OF_RANGE,
    ERROR_WRONG_NUMBER_OF_ARGS
};

typedef ret_type_t(*callback)(int, char*[]);
typedef void(*minicb)(double*, double);

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
        case ERROR_ZERO_VAL:
            printf("Value is zero.\n");
            break;
        case ERROR_ZERO_SIDE:
            printf("Side of triangle can`t be zero.\n");
            break;
        default:
            printf("ABORTED!!!");
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

    int len = 0, is_zero = 0;
    while (isdigit(*s)) {
        if (*s != '0') is_zero = 1;
        s++;
        if (len++ > 9) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') {
        if (!is_zero) return ERROR_ZERO_VAL;
        return SUCCESS;
    }
    return ERROR_NOT_NUMBER;
}

ret_type_t is_double(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') s++;

    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was)) {
        if (*s == '.')
            point_was += 1;
        s++;
        if (len++ > 10)
            return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
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

void ResOfEquation(double *arr, double eps) {
    double res1 = NULL, res2 = NULL;
    double discriminant = sqrt(arr[1] * arr[1] - 4 * arr[0] * arr[2]);
    if (fabs(arr[0]) < eps) {
        res1 = -arr[2] / arr[1];
        printf("Solution of the quadratic equation is x = %.10f.\n", res1);
        return;
    }
    res1 = (-arr[1] + discriminant) / (2 * arr[0]);
    res2 = (-arr[1] - discriminant) / (2 * arr[0]);
    if (discriminant > eps && fabs(arr[0]) > eps) {
        printf("Solution of the quadratic equation is x1 = %.10f, x2 = %.10f.\n", res1, res2);
        return;
    }
    if (fabs(discriminant) < eps) {
        printf("Solution of the quadratic equation is x = %.10f.\n", res1);
        return;
    }
    printf("There is no solution to the quadratic equation.\n");
}

void ObertkaOfEq(double *arr, double eps) {
    static int x = 1;
    printf("%d) Rotation: ", x);
    for (int i = 0; i <= 2; i++) {
        printf("%.10f ", arr[i]);
    }
    printf("\n");
    ResOfEquation(arr, eps);
    ++x;
}

void permute(double *arr, int l, int r, double eps, minicb ResFunc) {
    if (l == r) {
        ResFunc(arr, eps);
    } else {
        for (int i = l; i <= r; i++) {
            swap(&arr[l], &arr[i]);
            permute(arr, l + 1, r, eps, ResFunc);
            swap(&arr[l], &arr[i]);
        }
    }
}

ret_type_t checker(int cnt, char* input[], double* kef, double* eps) {
    if (cnt != 6) {
        return ERROR_WRONG_NUMBER_OF_ARGS;
    }
    ret_type_t code;
    for (int i = 2; i < cnt; ++i) {
        if ((code = is_double(input[i]))) {
            return code;
        }
    }
    *eps = atof(input[2]);
    if (*eps == 0) {
        return ERROR_ZERO_VAL;
    }
    for (int i = 3; i < cnt; ++i) {
        kef[i - 3] = atof(input[i]);
    }
    return SUCCESS;
}

ret_type_t funcForQ(int cnt, char* input[]) {
    double kef[3];
    double eps;
    ret_type_t code;
    if ((code = checker(cnt, input, kef, &eps))) {
        return code;
    }
    for (int i = 3; i < cnt; ++i) {
        if (atoi(input[i]) == 0) {
            return ERROR_ZERO_SIDE;
        }
    }
    permute(kef, 0, 2, eps, ObertkaOfEq);

    return SUCCESS;
}

ret_type_t funcForM(int cnt, char* input[]) {
    if (cnt != 4) {
        return ERROR_WRONG_NUMBER_OF_ARGS;
    }
    ret_type_t code;
    for (int i = 2; i < cnt; ++i) {
        if ((code = is_int(input[i]))) {
            return code;
        }
    }
    int first = Atoi(input[2]);
    int second = Atoi(input[3]);
    if ((first % second) == 0) {
        printf("The first number is divisible by the second.\n");
    } else {
        printf("The first number is not divisible by the second.\n");
    }
    return SUCCESS;
}

void CheckOfSides(double *arr, double eps) {
    static int flag = 0, iter_cnt = 0;
    ++iter_cnt;
    if (flag) return;
    if (fabs(arr[0] * arr[0] - arr[1] * arr[1] - arr[2] * arr[2]) < eps) {
        flag = 1;
        printf("A right-angled triangle can have sides of such length.\n");
        return;
    }
    if (iter_cnt == 6) {
        printf("A right-angled triangle cannot have sides of such length.\n");
    }
}

ret_type_t funcForT(int cnt, char* input[]) {
    double kef[3];
    double eps;
    ret_type_t code;
    if ((code = checker(cnt, input, kef, &eps))) {
        return code;
    }
    permute(kef, 0, 2, eps, CheckOfSides);
    return SUCCESS;
}

int main(int argc, char* argv[]) {

    const char* flags[] = { "-q", "/q", "-m", "/m", "-t", "/t" };
    callback cbs[] = { &funcForQ, &funcForM, &funcForT};

    int ret = findFlag(argv[1], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1)
    {
        printf("THIS FLAG DOES NOT EXIST %s\n", argv[1]);
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
    }

    return 0;
}
