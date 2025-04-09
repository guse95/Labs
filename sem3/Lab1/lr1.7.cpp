#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCESS,
    ERROR_NO_VALUE,
    ERROR_NOT_PATH,
    MEMORY_ALLOCATION_ERROR,
    ERROR_WRONG_NUMBER_OF_ARGS,
    FILE_OPENING_ERROR
};

typedef void(*callback)(FILE*, FILE*);

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

void funcForR(FILE* infile1, FILE* infile2, FILE* outfile) {
    int word_number = 1;
    FILE * curFile = infile1;
    char cur_el = fgetc(curFile);
    while (cur_el != EOF) {
        while (!(isalnum(cur_el)) && cur_el != EOF) {
            cur_el = fgetc(curFile);
        }
        if (cur_el == EOF) {
            break;
        }
        while (isalnum(cur_el)) {
            fputc(cur_el, outfile);
            cur_el = fgetc(curFile);
        }
        fputc(' ', outfile);
        ++word_number;
        curFile = (word_number % 2 == 1) ? infile1 : infile2;
    }
    if ((cur_el = fgetc(infile1)) != EOF) {
        curFile = infile1;
    } else {
        cur_el = fgetc(infile2);
        curFile = infile2;
    }
    while (cur_el != EOF) {
        while (!isalnum(cur_el) && cur_el != EOF) {
            cur_el = fgetc(curFile);
        }
        while (isalnum(cur_el) && cur_el != ' ') {
            fputc(cur_el, outfile);
            cur_el = fgetc(curFile);
        }
        fputc(' ', outfile);
    }
    printf("SUCCESS.");
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

void funcForA(FILE* infile, FILE* outfile) {
    int word_number = 1;
    char cur_el = fgetc(infile);
    while (cur_el != EOF) {
        while (!(isalnum(cur_el)) && cur_el != EOF) {
            cur_el = fgetc(infile);
        }
        if (cur_el == EOF) {
            break;
        }
        while (isalnum(cur_el)) {
            if (word_number % 10 == 0) {
                if(isalpha(cur_el)) {
                    FromDesToSist(4, tolower(cur_el), outfile);
                } else {
                    fputc(cur_el, outfile);
                }
            } else {
                if (word_number % 2 == 0) {
                    fputc(tolower(cur_el), outfile);
                }
                else if (word_number % 5 == 0) {
                    FromDesToSist(8, tolower(cur_el), outfile);
                }
                else {
                    fputc(cur_el, outfile);
                }
            }
            cur_el = fgetc(infile);
        }
        fputc(' ', outfile);
        ++word_number;
    }
    printf("SUCCESS.");
}

int main(int argc, char* argv[]) {

    const char* flags[] = { "-r", "/r", "-a", "/a" };

    int ret = findFlag(argv[1], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1)
    {
        printf("THIS FLAG DOES NOT EXIST %s\n", argv[1]);
        return -1;
    }
    else
    {
        if (argc < 4) {
            HandlingError(ERROR_WRONG_NUMBER_OF_ARGS);
            return ERROR_WRONG_NUMBER_OF_ARGS;
        }

        if (ret_type_t code = path_checker(argv[2])) {
            HandlingError(code);
            return code;
        }

        if (ret_type_t code = path_checker(argv[argc - 1])) {
            HandlingError(code);
            return code;
        }

        if (strcmp(argv[2], argv[argc - 1]) == 0) {
            printf("Input file is the same as the output file.\n");
            return -1;
        }

        char* inputFileName1 = (char*)malloc((strlen(argv[2]) + 1));
        if (inputFileName1 == NULL) {
            HandlingError(MEMORY_ALLOCATION_ERROR);
            return MEMORY_ALLOCATION_ERROR;
        }
        strcpy(inputFileName1, argv[2]);

        char* outputFileName = (char*)malloc(strlen(argv[argc - 1]) + 1);
        if (outputFileName == NULL) {
            HandlingError(MEMORY_ALLOCATION_ERROR);
            free(inputFileName1);
            return MEMORY_ALLOCATION_ERROR;
        }
        strcpy(outputFileName, argv[argc - 1]);

        FILE *infile1;
        FILE *outfile;
        infile1 = fopen(inputFileName1, "r");
        if (infile1 == NULL) {
            HandlingError(FILE_OPENING_ERROR);
            free(inputFileName1);
            free(outputFileName);
            return FILE_OPENING_ERROR;
        }
        outfile = fopen(outputFileName, "w");
        if (outfile == NULL) {
            HandlingError(FILE_OPENING_ERROR);
            fclose(infile1);
            free(inputFileName1);
            free(outputFileName);
            return FILE_OPENING_ERROR;
        }
        if (ret == 0) {
            if (argc < 5) {
                HandlingError(ERROR_WRONG_NUMBER_OF_ARGS);
                fclose(infile1);
                fclose(outfile);
                free(inputFileName1);
                free(outputFileName);
                return ERROR_WRONG_NUMBER_OF_ARGS;
            }
            ret_type_t code;
            if ((code = path_checker(argv[3]))) {
                HandlingError(code);
                fclose(infile1);
                fclose(outfile);
                free(inputFileName1);
                free(outputFileName);
                return code;
            }

            char* inputFileName2 = (char*)malloc(strlen(argv[3]) + 1);
            if (inputFileName2 == NULL) {
                HandlingError(MEMORY_ALLOCATION_ERROR);
                fclose(infile1);
                fclose(outfile);
                free(inputFileName1);
                free(outputFileName);
                return MEMORY_ALLOCATION_ERROR;
            }
            strcpy(inputFileName2, argv[3]);
            FILE* infile2;

            infile2 = fopen(inputFileName2, "r");
            if (infile2 == NULL) {
                HandlingError(FILE_OPENING_ERROR);
                fclose(infile1);
                fclose(outfile);
                free(inputFileName1);
                free(inputFileName2);
                free(outputFileName);
                return FILE_OPENING_ERROR;
            }
            funcForR(infile1, infile2, outfile);
            fclose(infile2);
            free(inputFileName2);
        } else {
            funcForA(infile1, outfile);
        }
        fclose(infile1);
        fclose(outfile);
        free(inputFileName1);
        free(outputFileName);
    }
    return SUCCESS;
}