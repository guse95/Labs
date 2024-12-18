#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

enum ret_type_t{
    SUCCESS,
    ERROR_NAN,
    ERROR_FULL,
    ERROR_ARGS,
    ERROR_MALLOC,
    ERROR_OPEN_FILE,
    ERROR_ITERATIONS_LIMIT,
    ERROR_NO_SOLVE
};

void logErrors(int code) {
    switch (code) {
        case ERROR_NAN: printf("Found not a number\n"); break;
        case ERROR_FULL: printf("Overflow detected\n"); break;
        case ERROR_ARGS: printf("Wrong arguments\n"); break;
        case ERROR_MALLOC: printf("Failed to malloc\n"); break;
        case ERROR_OPEN_FILE: printf("Failed to open file\n"); break;
        case ERROR_ITERATIONS_LIMIT: printf("Too many iterations. Time to shut up\n"); break;
        case ERROR_NO_SOLVE: printf("There is no solvement in your interval\n"); break;
        default: printf("UNKNOWN ERROR CODE\n"); break;
    }
}

int Roman(const char *roman) {
    struct Rom {
        int value;
        const char *symbol;
    }
    roman_map[] = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    int result = 0, i = 0;
    while (*roman) {
        if (strncmp(roman, roman_map[i].symbol, strlen(roman_map[i].symbol)) == 0) {
            result += roman_map[i].value;
            roman += strlen(roman_map[i].symbol);
        } else {
            i++;
        }
    }
    return result;
}

unsigned int Fidonachi(const char *fib_num) {
    unsigned int fib[64], i = 0;
    fib[0] = 1; fib[1] = 2;
    for (i = 2; i < 64; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    unsigned int num = 0, index = 0;
    size_t len = strlen(fib_num);

    for (int j = len - 2; j >= 0; j--) {
        if (fib_num[j] == '1') {
            num += fib[index];
        }
        index++;
    }
    return num;
}

int FromSistToDes(const char *num_str, int base) {
    int result = 0;
    int is_negative = (*num_str == '-');
    if (is_negative) num_str++;

    while (*num_str) {
        char c = tolower(*num_str);
        int value = (c >= '0' && c <= '9') ? c - '0' : c - 'a' + 10;
        result = result * base + value;
        num_str++;
    }
    return is_negative ? -result : result;
}

int overfscanf(FILE *stream, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    int total_read = 0;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'R': {
                    format++;
                    if (*format == 'o') {
                        int *num = va_arg(args, int *);
                        fscanf(stream, "%255s", buffer);
                        *num = Roman(buffer);
                        total_read++;
                    }
                    break;
                }
                case 'Z': {
                    format++;
                    if (*format == 'r') {
                        unsigned int *num = va_arg(args, unsigned int *);
                        fscanf(stream, "%255s", buffer);
                        *num = Fidonachi(buffer);
                        total_read++;
                    }
                    break;
                }
                case 'C': {
                    format++;
                    int *num = va_arg(args, int *);
                    int base = va_arg(args, int);
                    if (base < 2 || base > 36) base = 10;
                    fscanf(stream, "%255s", buffer);
                    *num = FromSistToDes(buffer, base);
                    total_read++;
                    break;
                }
                default:
                    ungetc('%', stream);
                    total_read += vfscanf(stream, format - 1, args);
            }
        } else {
            ungetc(*format, stream);
            total_read += vfscanf(stream, format, args);
        }
        format++;
    }

    va_end(args);
    return total_read;
}

int oversscanf(const char *str, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    int total_read = 0;

    while (*format) {
        if (*format == '%') {
            format++;
            switch (*format) {
                case 'R': {
                    format++;
                    if (*format == 'o') {
                        int *num = va_arg(args, int *);
                        fscanf(stdin, "%255s", buffer);
                        *num = Roman(buffer);
                        total_read++;
                    }
                    break;
                }
                case 'Z': {
                    format++;
                    if (*format == 'r') {
                        unsigned int *num = va_arg(args, unsigned int *);
                        fscanf(stdin, "%255s", buffer);
                        *num = Fidonachi(buffer);
                        total_read++;
                    }
                    break;
                }
                case 'C': {
                    format++;
                    int *num = va_arg(args, int *);
                    int base = va_arg(args, int);
                    if (base < 2 || base > 36) base = 10;
                    fscanf(stdin, "%255s", buffer);
                    *num = FromSistToDes(buffer, base);
                    total_read++;
                    break;
                 }
                default:
                    ungetc('%', stdin);
                    //--format;
                    total_read += fscanf(stdin, format, args);
            }
        } else {
            ungetc(*format, stdin);
            total_read += fscanf(stdin, format, args);
        }
        format++;
    }

    va_end(args);
    return total_read;
}

int main() {
    int num;
    unsigned int unum;
    char input[256];

    printf("Enter a Roman numeral: ");
    oversscanf(input, "%Ro", &num);
    printf("Parsed Roman numeral: %d\n", num);

    printf("Enter Fibonacci number: ");
    oversscanf(input, "%Zr", &unum);
    printf("Parsed Fibonacci-Zec: %u\n", unum);

    printf("Enter a number in base 16: ");
    oversscanf(input, "%Cv", &num, 16);
    printf("Parsed number: %d\n", num);

    printf("Enter a number in base 16 (uppercase): ");
    oversscanf(input, "%CV", &num, 16);
    printf("Parsed number: %d\n", num);

    oversscanf(input, "%d", &num);
    printf("Just number: %d\n", num);

    return SUCCESS;
}