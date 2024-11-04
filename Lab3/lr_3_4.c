#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCESS,
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_NOT_STR,
    ERROR_NO_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NEGATIVE_VALUE,
    ERROR_NOT_PATH,
    FILE_OPENING_ERROR,
    ERROR_SAME_IN_AND_OUT_FILES,
    MEMORY_ALLOCATION_ERROR,
    FILE_ENDED
};

void HandlingError(int code) {
    switch (code) {
        case ERROR_NEGATIVE_VALUE:
            printf("Negative value was entered.\n");
            break;
        case ERROR_NO_VALUE:
            printf("Not number was entered.\n");
            break;
        case ERROR_NOT_NUMBER:
            printf("Value is not a number.\n");
            break;
        case ERROR_TOO_LONG_STR:
            printf("Too long string was entered.\n");
            break;
        case FILE_OPENING_ERROR:
            printf("File opening error.\n");
            break;
        case ERROR_NOT_PATH:
            printf("Value is not a path.\n");
            break;
        case ERROR_SAME_IN_AND_OUT_FILES:
            printf("Input and output files can not be equal.\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            printf("Memory allocation error.\n");
            break;
        case ERROR_NOT_STR:
            printf("String can not be name or surname.\n");
            break;
        default:
            break;
    }
}

typedef int(*callback)(char*);

struct String {
    int len;
    char* str;
};

int toStr(struct String *newStr, char * ptr) {
    newStr->len = 0;
    char* s = ptr;
    while (*s++) newStr->len++;
    newStr->str = (char*) malloc(newStr->len * sizeof(char));
    if (newStr->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < newStr->len; ++i) {
        newStr->str[i] = ptr[i];
    }
    return SUCCESS;
}

void StrClear(struct String *str) {
    free(str->str);
    str->len = 0;
}

int StrCmp(struct String str1, struct String str2) {
    if (str1.len != str2.len) {
        printf("%d  %d\n", str1.len, str2.len);
        return (str1.len > str2.len) ? 1 : -1;
    }
    for (int i = 0; i < str1.len; i++) {
        if (str1.str[i] != str2.str[i]) {
            return (str1.str[i] > str2.str[i]) ? 1 : -1;
        }
    }
    return 0;
}

int StrEq(struct String str1, struct String str2) {
    if (str1.len != str2.len) {
        return 0;
    }
    for (int i = 0; i < str1.len; i++) {
        if (str1.str[i] != str2.str[i]) {
            return 0;
        }
    }
    return 1;
}

int StrCopy (struct String* new, struct String old) {
    StrClear(new);
    new->len = old.len;
    new->str = (char*) malloc(new->len * sizeof(char));
    if (new->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < new->len; i++) {
        new->str[i] = old.str[i];
    }
    return SUCCESS;
}

int StrCopyToNew (struct String *new, struct String old) {
    new->len = old.len;
    new->str = (char*) malloc(new->len * sizeof(char));
    if (new->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < new->len; i++) {
        new->str[i] = old.str[i];
    }
    return SUCCESS;
}

int StrConc(struct String * str1, struct String str2) {
    char* ptr = (char *) realloc(str1->str, (str1->len + str2.len) * sizeof(char));
    if (ptr == NULL) {
        printf("Memory reallocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    str1->str = ptr;
    for (int i = 0; i < str2.len; i++) {
        str1->str[i + str1->len] = str2.str[i];
    }
    str1->len += str2.len;
    return SUCCESS;
}


struct Address {
    struct String town;
    struct String street;
    unsigned int house;
    struct String corp;
    unsigned int flat;
    int ind[6];
};

struct Mail {
    struct Address address;
    float weight;
    int postInd[14];
    struct String creature[19];
    struct String handing[19];
};

struct Post {
    struct Address postOffice;
    struct Mail *mails;
};


int main(int argc, char* argv[]) {
    char *test = "weekend";
    char *test2 = "weekend";
    struct String str;
    toStr(&str, test);
    struct String strnew;
    toStr(&strnew, test2);
    StrCopy(&strnew, str);
    printf("%d\n", StrEq(strnew, str));
    StrConc(&strnew, strnew);
    for (int i = 0; i < strnew.len; i++) {
        printf("%c ", strnew.str[i]);
    }
}