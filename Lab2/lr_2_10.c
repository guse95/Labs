#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

enum {
    MEMORY_ALLOCATION_ERROR,
    SUCCESS
} CODES;

void ValidateCode(const int code) {
    switch (code) {
        case MEMORY_ALLOCATION_ERROR: {
            printf("Malloc failed\n"); break;
        }
        case SUCCESS: {
            break;
        }
        default: {
            printf("Unknown error code\n"); break;
        }
    }
}

int** RightTriangle(int num) {
    int** coefs_str_num = malloc((num + 1) * sizeof(int*));
    if (coefs_str_num == NULL) {
        return NULL;
    }
    for (int i = 0; i <= num; i++) {
        coefs_str_num[i] = malloc((i + 1) * sizeof(int));
        int* tek_str = coefs_str_num[i];
        if (tek_str == NULL) {
            free(coefs_str_num);
            return NULL;
        }
        tek_str[0] = 1;
        for(int j = 1; j < i; j++) {
            tek_str[j] = coefs_str_num[i - 1][j - 1] + coefs_str_num[i - 1][j];
        }
        tek_str[i] = 1;
    }
    return coefs_str_num;
}

int Kefs(double a, double* res_kefs, int n, ...) {
    va_list args;
    va_start (args, n);
    double* cur_kefs = (double*)malloc(sizeof(double) * (n + 1));
    if (cur_kefs == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    for(int i = 0; i <= n; i++) {
        cur_kefs[i] = va_arg(args, double);
        res_kefs[i] = cur_kefs[i];
    }
    int** RightTriangle_cur = RightTriangle(n);
    if (RightTriangle_cur == NULL) {
        free(cur_kefs);
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = n; i > 0; i--) {
        int* cur_triangle = RightTriangle_cur[i];
        double cur_a = a;
        for (int j = i - 1; j >= 0; j--) {
            res_kefs[j] += cur_triangle[j] * cur_a * cur_kefs[i];
            cur_a *= a;
        }
    }
    for (int i = 0; i <= n; i++) {
        printf("%lf ", res_kefs[i]);
    }
    printf("\n");
    va_end(args);
    free(RightTriangle_cur
        );
    free(cur_kefs);
    return SUCCESS;
}

int main() {
    int n = 3;
    double* res_coefs = (double*)malloc((n + 1) * sizeof(double));
    if (res_coefs == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    ValidateCode(Kefs(2.0, res_coefs, n, 4.0, 3.0, 2.0, 1.0));
    ValidateCode(Kefs(3.0, res_coefs, n, 7.0, 3.5, 2.7, 1.4));
    ValidateCode(Kefs(5.0, res_coefs, n, 17.1, 2.1, 1.5, 6.1));
    free(res_coefs);
}