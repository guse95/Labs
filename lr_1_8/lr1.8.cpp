#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <windows.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_NOT_PATH,
    ERROR_TOO_LONG_STR,
    MEMORY_ALLOCATION_ERROR,
    ERROR_WRONG_NUMBER_OF_ARGS,
    FILE_OPENING_ERROR
};

typedef ret_type_t(*callback)(FILE*, FILE*);

void HandlingError(int code) {
    switch (code) {
        case SUCCESS:
            printf("Success.\n");
            break;
        case ERROR_NOT_PATH:
            printf("Value isn`t a path.\n");
            break;
        case ERROR_NO_VALUE:
            printf("No number was entered.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long lexeme was met in file.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        case ERROR_WRONG_NUMBER_OF_ARGS:
            printf("Wrong number of arguments.\n");
            break;
        case FILE_OPENING_ERROR:
            printf("File opening error.\n");
            break;
        default:
            printf("ABORTED!!!");
            break;
    }
}

ret_type_t path_checker(char* path) {
    char* ptr = path + strlen(path);
    while (*ptr != '.') {
        --ptr;
    }
    ++ptr;
    if(strcmp(ptr, "txt") != 0) {
        return ERROR_NOT_PATH;
    }
    while (isalpha(*ptr) || isdigit(*ptr) || *ptr == '-'
           || *ptr == '_' || *ptr == '.' || *ptr == '\\') {
        --ptr;
    }
    if (*ptr != ':' || !isupper(*(ptr - 1)) || *(ptr + 1) != '\\') {
        return ERROR_NOT_PATH;
    }
    return SUCCESS;
}

ret_type_t WorkWithFile(char* in, char* out, callback func) {
    FILE *infile;
    FILE *outfile;
    infile = fopen(in, "r");
    if (infile == NULL) {
        return FILE_OPENING_ERROR;
    }
    outfile = fopen(out, "w");
    if (outfile == NULL) {
        return FILE_OPENING_ERROR;
    }
    if (ret_type_t code = func(infile, outfile)) {
        fclose(infile);
        fclose(outfile);
        return code;
    }
    fclose(infile);
    fclose(outfile);
    return SUCCESS;
}

void FromDesToSist(const int base, int x, FILE* out){
    char buf[BUFSIZ], *pbuf = buf + BUFSIZ - 1, r;
    *pbuf-- = 0;
    while (x) {
        *pbuf-- = ((r = x % base) > 9) ? r - 10 + 'A' : r + '0';
        x /= base;
    }
    while (*++pbuf) {
        fprintf(out, "%c", *pbuf);
    }
}

int FromSistToDes(const int base, char* str, int len){
    int res = 0;
    char *pstr = str;
    int i = 0;
    while (i != len) {
        res = res * base + (isalpha(pstr[i]) ? toupper(pstr[i]) - 'A' + 10 : pstr[i] - '0');
        ++i;
    }
    return res;
}

ret_type_t solve(FILE* infile, FILE* outfile) {
    char cur_el = fgetc(infile);
    char* pArr, *ptr;

    while (cur_el != EOF) {
        int cnt = 2, r;
        pArr = (char*)malloc(cnt * sizeof(char));
        if (pArr == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        int i = 0, base = 0;
        int leading_zeros = 1;
        while (!(isalnum(cur_el)) && cur_el != EOF) {
            cur_el = fgetc(infile);
        }
        if (cur_el == EOF) {
            free(pArr);
            break;
        }
        while (isalnum(cur_el)) {
            if (cur_el != '0' && leading_zeros) {
                leading_zeros = 0;
            }
            if (leading_zeros) {
                cur_el = fgetc(infile);
                continue;
            }
            if (i == cnt) {
                cnt *= 2;
                ptr = (char*)realloc(pArr, cnt * sizeof(char));
                if (ptr == NULL) {
                    free(pArr);
                    return MEMORY_ALLOCATION_ERROR;
                }
                pArr = ptr;
            }
            pArr[i] = cur_el;

            if (isdigit(cur_el)) {
                base = ((r = cur_el - '0' + 1) > base) ? r : base;
            } else {
                base = ((r = toupper(cur_el) - 'A' + 11) > base) ? r : base;
            }
            if (!leading_zeros) {
                fprintf(outfile, "%c", cur_el);
            }
            cur_el = fgetc(infile);
            ++i;
        }

        int maxLen = 0, intLim = 100000000;
        while (intLim > 0) {
            intLim /= base;
            maxLen++;
        }
        if (i > maxLen) {
            free(pArr);
            return ERROR_TOO_LONG_STR;
        }

        fputc(' ', outfile);
        FromDesToSist(10, base, outfile);
        fputc(' ', outfile);
        int res = FromSistToDes(base, pArr, i);
        FromDesToSist(10, res, outfile);
        fputc('\n', outfile);
        free(pArr);
    }
    return SUCCESS;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        HandlingError(ERROR_WRONG_NUMBER_OF_ARGS);
        return ERROR_WRONG_NUMBER_OF_ARGS;
    }
    for (int i = 1; i <= 2; ++i) {
        if (ret_type_t code = path_checker(argv[i])) {
            HandlingError(code);
            return code;
        }
    }
    char* inputFileName = (char*)malloc((strlen(argv[1]) + 1));
    if (inputFileName == NULL) {
        HandlingError(MEMORY_ALLOCATION_ERROR);
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(inputFileName, argv[1]);

    char* outputFileName = (char*)malloc((strlen(argv[2]) + 1));
    if (outputFileName == NULL) {
        HandlingError(MEMORY_ALLOCATION_ERROR);
        return MEMORY_ALLOCATION_ERROR;
    }
    strcpy(outputFileName, argv[2]);

    ret_type_t code = WorkWithFile(inputFileName, outputFileName, solve);
    HandlingError(code);
    free(inputFileName);
    free(outputFileName);
    if (code) {
        return -1;
    }
    return SUCCESS;
}