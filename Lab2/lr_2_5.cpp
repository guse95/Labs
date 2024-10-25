#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NEGATIVE_VALUE,
    ERROR_TOO_LONG_STR,
    ERROR_ARGUMENR_OUT_OF_SS,
    ERROR_WRONG_REGISTR,
    MEMORY_ALLOCATION_ERROR
};

typedef ret_type_t(*callback)(FILE*, int);

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case ERROR_ARGUMENR_OUT_OF_SS:
            printf("Value is not a number in right SS.\n");
            break;
        case ERROR_WRONG_REGISTR:
            printf("Wrong register of number.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        default:
            printf("ABORTED!!!");
            break;
    }
}

int my_strcmp(const char *str1, const char *str2) {
    if (*str1 == '\0'){
        return 0;
    }
    while (*str1 != '\0' && *str2 != '\0') {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    if (*str1 == '\0' && *str2 != '\0') {
        return 0;
    }
    return 1;
}

int findFlag(const char* curarg, const char** flags, const char** standart, int size, int size_stand) {
    for (int i = 0; i < size; ++i)
    {
        if ((my_strcmp(curarg, flags[i])))
        {
            return i;
        }
    }
    for (int j = 0; j < size_stand; ++j)
    {
        if ((my_strcmp(curarg, standart[j])))
        {
            return (j + 10);
        }
    }
    return -1;
}

ret_type_t funcForRo(FILE* file, int arg) {
    if (arg < 0) {
        return ERROR_NEGATIVE_VALUE;
    }
    while (arg >= 1000) {
        arg -= 1000;
        fprintf(file, "M");
    }
    if (arg >= 900) {
        arg -= 900;
        fprintf(file, "CM");
    }
    if (arg >= 500) {
        arg -= 500;
        fprintf(file, "D");
    }
    if (arg >= 400) {
        arg -= 400;
        fprintf(file, "CD");
    }
    while (arg >= 100) {
        arg -= 100;
        fprintf(file, "C");
    }
    if (arg >= 90) {
        arg -= 90;
        fprintf(file, "XC");
    }
    if (arg >= 50) {
        arg -= 50;
        fprintf(file, "L");
    }
    if (arg >= 40) {
        arg -= 40;
        fprintf(file, "XL");
    }
    while (arg >= 10) {
        arg -= 10;
        fprintf(file, "X");
    }
    if (arg >= 9) {
        arg -= 9;
        fprintf(file, "IX");
    }
    if (arg >= 5) {
        arg -= 5;
        fprintf(file, "V");
    }
    if (arg >= 4) {
        arg -= 4;
        fprintf(file, "IV");
    }
    while (arg >= 1) {
        arg -= 1;
        fprintf(file, "I");
    }
    return SUCCESS;
}

unsigned int fibonachiLower(unsigned int upBound) {
    unsigned int prev = 1, cur = 1, buf;
    while (cur <= upBound) {
        buf = cur;
        cur = prev + cur;
        prev = buf;
    }
    return prev;
}

ret_type_t funcForZr(FILE* file, unsigned int arg) {
    unsigned int numbs[BUFSIZ];
    int ind = 0;
    while (arg) {
        unsigned int fib_max = fibonachiLower(arg);
        numbs[ind] = fib_max;
        ++ind;
        arg -= fib_max;
    }
    for (int i = ind - 1; i >= 0; --i) {
        fprintf(file, "%d", numbs[i]);
    }
    fprintf(file, "%d", 1);
    return SUCCESS;
}

ret_type_t funcForC(FILE* file, int base, int numb, int registr) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    char buf[BUFSIZ], *pbuf = buf + BUFSIZ - 1, r;
    *pbuf-- = 0;
    while (numb) {
        if ((r = numb % base) > 9) {
            *pbuf-- = r - 10 + ((registr == 1) ? 'A': 'a');
        } else {
            *pbuf-- = r + '0';
        }
        numb /= base;
    }
    while (*++pbuf) {
        fprintf(file, "%c", *pbuf);
    }
    return SUCCESS;
}

ret_type_t funcForT (FILE* file, int base, char* numb, int registr) {
    if (base < 2 || base > 36) {
        base = 10;
    }
    int res = 0;
    char *pstr = numb;
    int len = 0;
    int maxLen = 0, intLim = 100000000;
    while (intLim > 0) {
        intLim /= base;
        maxLen++;
    }
    while (*pstr) {
        if ((registr) ? isupper(*pstr) : islower(*pstr)) {
            return ERROR_WRONG_REGISTR;
        }
        if ((toupper(*pstr) - 'A') > base) {
            return ERROR_ARGUMENR_OUT_OF_SS;
        }
        if (maxLen < len) {
            return ERROR_TOO_LONG_STR;
        }
        if (isalpha(*pstr)) {
            res = res * base + toupper(*pstr) - 'A' + 10;
        } else {
            res = res * base + *pstr - '0';
        }
        ++pstr;
        ++len;
    }
    fprintf(file, "%d", res);
    return SUCCESS;
}

ret_type_t funcForM(FILE* file, void* numb, int size) {
    char *dump = (char *)malloc((9 * size) * sizeof(char));
    if (!dump){
        return MEMORY_ALLOCATION_ERROR;
    }
    char *ptr = (char*)numb;
    int ind = 0;
    for (int i = 0; i < size; ++i) {
        for (int j = 7; j >= 0; --j) {
            dump[ind++] = (ptr[i] & (1 << j)) ? '1' : '0';
        }
        dump[ind++] = ' ';
    }
    dump[--ind] = '\0';
    fprintf(file, "%s", dump);
    free(dump);
    return SUCCESS;
}

int overfprintf(FILE* file, const char* format, ...) {
    const char *flags[] = {"%Ro", "%Zr", "%Cv", "%CV", "%to", "%TO", "%mi", "%mu", "%md", "%mf"};
    const char *standart[] = { "%c", "%s", "%d", "%i", "%o", "%x", "%X", "%u", "%f", "%F", "%e", "%E", "%a", "%A", "%g", "%G", "%n", "%p"};
    va_list args;
    va_start(args, format);
    while (*format != '\0') {
        if (*format != '%' || *(format + 1) == '%') {
            printf("|%c|\n", *format);
            fprintf(file, "%c", *format);
            ++format;
            continue;
        }
        printf("%s\n", format);
        int flag = findFlag(format, flags, standart, 10, 18);
        ++format;
        ret_type_t code;
        printf("%d\n", flag);
        switch (flag) {
            case 0: {
                int arg = va_arg(args, int);
                if((code = funcForRo(file, arg))){
                    HandlingError(code);
                    return code;
                }
                break;
            }
            case 1: {
                unsigned int arg1 = va_arg(args, unsigned int);
                funcForZr(file, arg1);
                break;
            }
            case 2: {
                int arg = va_arg(args, int);
                int base = va_arg(args, int);
                funcForC(file, base, arg, 0);
                break;
            }
            case 3: {
                int arg = va_arg(args, int);
                int base = va_arg(args, int);
                funcForC(file, base, arg, 1);
                break;
            }
            case 4: {
                char* arg = va_arg(args, char*);
                int base = va_arg(args, int);
                if((code = funcForT(file, base, arg, 1))){
                    HandlingError(code);
                    return code;
                }
                break;
            }
            case 5: {
                char* arg = va_arg(args, char*);
                int base = va_arg(args, int);
                if((code = funcForT(file, base, arg, 0))){
                    HandlingError(code);
                    return code;
                }
                break;
            }
            case 6: {
                int arg = va_arg(args, int);
                funcForM(file, &arg, sizeof(int));
                break;
            }
            case 7: {
                unsigned int arg = va_arg(args, unsigned int);
                funcForM(file, &arg, sizeof(unsigned int));
                break;
            }
            case 8: {
                double arg = va_arg(args, double);
                funcForM(file, &arg, sizeof(double));
                break;
            }
            case 9: {
                float arg = va_arg(args, float);
                funcForM(file, &arg, sizeof(float));
                break;
            }
            case -1: {
                fprintf(file, "%c", *--format);
                fprintf(file, "%c", *++format);
                break;
            }
            default: {
                if (flag > 9) {
                    char *arg = va_arg(args, char*);
                    printf("%d  %s  ", flag, standart[flag - 10]);
                    fprintf(file, standart[flag - 10], arg);
                    printf("%s\n", format - 1);
                    --format;
                    break;
                }
                printf("smth went wrong.\n");
                break;
            }
        }
        format += 2;
    }
    va_end(args);
    return SUCCESS;
}

int main() {
    FILE * file;
    file = fopen("D:\\out.txt", "w");
    if (file == NULL) {
        printf("Opening file ERROR\n");
        return -1;
    }

    overfprintf(file, "wmvnw %Zrmld ,dmmm%d   / / / /. .%%%Cv    |", 10, 52, 12345, 16);

    fclose(file);
}
