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

void Adding_prefix(char* prefix, char* outFile, char* inFile) {
    char* ptr_pref = prefix;
    char* ptr = inFile + strlen(inFile);
    while (*ptr != '\\') {
        --ptr;
    }
    ++ptr;
    while (inFile != ptr) {
        *outFile++ = *inFile++;
    }
    while (*ptr_pref != '\0') {
        *outFile++ = *ptr_pref++;
    }
    while ((*outFile++ = *ptr++));
}

ret_type_t WorkWithFile(char* in, char* out, callback func) {
    FILE *infile;
    FILE *outfile;
    infile = fopen(in, "r");
    if (infile == nullptr) {
        return FILE_OPENING_ERROR;
    }
    outfile = fopen(out, "w");
    if (outfile == nullptr) {
        return FILE_OPENING_ERROR;
    }
    func(infile, outfile);

    fclose(infile);
    fclose(outfile);
    return SUCCESS;
}

void funcForD(FILE* infile, FILE* outfile) {
    char cur_el;
    while ((cur_el = fgetc(infile)) != EOF) {
        if (!isdigit(cur_el)) {
            fputc((char)cur_el, outfile);
        }
    }
}

void funcForI(FILE* infile, FILE* outfile) {
    char cur_el;
    int cnt = 0;
    while ((cur_el = fgetc(infile)) != EOF) {
        if (isalpha(cur_el)) {
            cnt++;
        }
        if (cur_el == '\n') {
            fprintf(outfile, "%d\n", cnt);
            cnt = 0;
        }
    }
    fprintf(outfile, "%d\n", cnt);
}

void funcForS(FILE* infile, FILE* outfile) {
    char cur_el;
    int cnt = 0;
    while ((cur_el = fgetc(infile)) != EOF) {
        if (!isalnum(cur_el) && cur_el != ' ') {
            cnt++;
        }
        if (cur_el == '\n') {
            fprintf(outfile, "%d\n", cnt);
            cnt = 0;
        }
    }
    fprintf(outfile, "%d\n", cnt);
}

void funcForA(FILE* infile, FILE* outfile) {
    char cur_el;
    while ((cur_el = fgetc(infile)) != EOF) {
        if (!isdigit(cur_el)) {
            fprintf(outfile, "%X", cur_el);
        } else {
            fputc((char)cur_el, outfile);
        }
    }
}

int main(int argc, char* argv[]) {

    const char* flags[] = { "-d", "/d", "-nd", "/nd","-i", "/i",  "-ni", "/ni",
                            "-s", "/s", "-ns", "/ns", "-a", "/a","-na", "/na" };
    callback cbs[] = { &funcForD, &funcForI, &funcForS, &funcForA };

    int ret = findFlag(argv[1], flags, sizeof(flags) / sizeof(char*));
    if (ret == -1)
    {
        printf("THIS FLAG DOES NOT EXIST %s\n", argv[1]);
        return -1;
    }
    else
    {
        int findCbsInt = ret / 4;
        callback find = cbs[findCbsInt];
        if (ret_type_t code = path_checker(argv[2])) {
            HandlingError(code);
            return code;
        }

        char* inputFileName = (char*)malloc((strlen(argv[2]) + 1));
        if (inputFileName == nullptr) {
            HandlingError(MEMORY_ALLOCATION_ERROR);
            return MEMORY_ALLOCATION_ERROR;
        }
        char* outputFileName;
        strcpy(inputFileName, argv[2]);

        if (ret % 4 > 1) {
            if (ret_type_t code = path_checker(argv[3])) {
                HandlingError(code);
                free(inputFileName);
                return code;
            }
            if (argc != 4) {
                HandlingError(ERROR_WRONG_NUMBER_OF_ARGS);
                return ERROR_WRONG_NUMBER_OF_ARGS;
            }

            outputFileName = (char*)malloc(strlen(argv[3]) + 1);
            if (outputFileName == nullptr) {
                HandlingError(MEMORY_ALLOCATION_ERROR);
                return MEMORY_ALLOCATION_ERROR;
            }
            strcpy(outputFileName, argv[3]);
        } else {
            if (argc != 3) {
                HandlingError(ERROR_WRONG_NUMBER_OF_ARGS);
                return ERROR_WRONG_NUMBER_OF_ARGS;
            }
            char prefix[] = "out_";
            outputFileName = (char*)malloc(strlen(argv[2]) + strlen(prefix) + 1);
            if (outputFileName == nullptr) {
                HandlingError(MEMORY_ALLOCATION_ERROR);
                return MEMORY_ALLOCATION_ERROR;
            }
            Adding_prefix(prefix, outputFileName, inputFileName);
        }

        ret_type_t code = WorkWithFile(inputFileName, outputFileName, find);
        HandlingError(code);
        free(inputFileName);
        free(outputFileName);
        if (code) {
            return -1;
        }
    }
    return SUCCESS;
}