#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

int * GetPrimeDividers(int num) {
    int size = 10;
    int * primeDividers = (int *)malloc(size * sizeof(int));
    if (primeDividers == NULL) {
        printf("Malloc failed\n");
        return NULL;
    }

    int ind = 0;
    if (num % 2 == 0) {
        primeDividers[ind] = 2;
        num /= 2;
        ++ind;
        while (num % 2 == 0) {
            num /= 2;
        }
    }

    for (int i = 3; i * i <= num; i += 2) {
        if (num % i == 0) {
            if (ind == size) {
                size *= 2;
                int *p = (int *) realloc(primeDividers, size * sizeof(int));
                if (p == NULL) {
                    printf("Realloc failed\n");
                    free(primeDividers);
                    return NULL;
                }
                primeDividers = p;
            }
            primeDividers[ind] = i;
            num /= i;
            ind++;
            while (num % i == 0) {
                num /= i;
            }
        }
    }

    if (num > 2) {
        if (ind == size) {
            size *= 2;
            int *p = (int *)realloc(primeDividers, size * sizeof(int));
            if (p == NULL) {
                printf("Realloc failed\n");
                free(primeDividers);
                return NULL;
            }
            primeDividers = p;
        }
        primeDividers[ind] = num;
        ind++;
    }

    if (ind == size) {
        size++;
        int *p = (int *)realloc(primeDividers, size * sizeof(int));
        if (p == NULL) {
            printf("Realloc failed\n");
            free(primeDividers);
            return NULL;
        }
        primeDividers = p;
    }
    primeDividers[ind] = 0;
    int i = 0;
    while (primeDividers[i] != 0) {
        printf("%d ", primeDividers[i]);
        ++i;
    }
    printf("\n");
    return primeDividers;
}

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

void HasFinalRepresentation(int base, int count, ...) {
    if (base < 2 || base > 36) {
        printf("Invalid base\n");
        return;
    }
    int * BaseDividers = GetPrimeDividers(base);

    va_list arg;
    va_start(arg, count);

    for (int i = 0; i < count; ++i) {
        double num = va_arg(arg, double);

        if (num <= 0 || num >= 1) {
            printf("Number %lf is invalid. (numbers must be from 0 to 1)\n", num);
            continue;
        }
        int numerator = (int)(num * 1e8);
        int denominator = 100000000;

        int GCD = gcd(numerator, denominator);
        numerator /= GCD;
        denominator /= GCD;

        int * numDividers = GetPrimeDividers(denominator);
        int * numPtr = numDividers;

        int isFinal = 1;
        while (*numPtr != 0) {
            int found = 0;
            int div = *numPtr;
            int * p = BaseDividers;

            while (*p != 0) {
                if (div == *p) {
                    found = 1;
                    break;
                }
                p++;
            }
            if (!found) {
                isFinal = 0;
                break;
            }
            numPtr++;
        }

        if (isFinal) {
            printf("Number %lf has final repr\n", num);
        } else {
            printf("Number %lf does not have final repr\n", num);
        }
        free(numDividers);
    }
    va_end(arg);
    free(BaseDividers);
}

int main() {
    HasFinalRepresentation(3, 4, 0.2, 0.25, 0.666, 0.5);
}