#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <time.h>
#include <stdlib.h>


enum ret_type_t {
    SUCCESS,
    ERROR_WRONG_NUMB_OF_ARGS,
    ERROR_NOT_STR,
    ERROR_NO_VALUE,
    ERROR_NOT_NUMBER,
    ERROR_TOO_LONG_STR,
    ERROR_NEGATIVE_VALUE,
    WRONG_SIZE_OF_INDEX,
    WRONG_FORMAT,
    ERROR_CANT_BE_TIME,
    MEMORY_ALLOCATION_ERROR,
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
        case WRONG_SIZE_OF_INDEX:
            printf("Wrong size of index.\n");
            break;
        case WRONG_FORMAT:
            printf("Wrong format of data.\n");
            break;
        case ERROR_CANT_BE_TIME:
            printf("String can not be time.\n");
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

int is_number(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') {
        return ERROR_NEGATIVE_VALUE;
    }

    int len = 0;
    while (isdigit(*s)) {
        s++;
        if (len++ > 18) return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

int is_float(const char* s) {
    if (*s == '\0') return ERROR_NO_VALUE;

    while (*s == ' ') s++;

    if (*s == '-') {
        return ERROR_NEGATIVE_VALUE;
    }

    int len = 0;
    int point_was = 0;
    while (isdigit(*s) || (*s == '.' && !point_was)) {
        if (*s == '.')
            point_was += 1;
        s++;
        if (len++ > 20)
            return ERROR_TOO_LONG_STR;
    }

    if (*s == '\0') return SUCCESS;
    return ERROR_NOT_NUMBER;
}

int is_str(const char* s) {
    while (*s) {
        if (!isalpha(*s)) {
            return ERROR_NOT_STR;
        }
        s++;
    }
    return SUCCESS;
}

int is_time(const char* s) {
//    printf("%s\n", s);
    if (!isdigit(*s++) || !isdigit(*s) || ((*(s - 1) - '0') * 10 + *s - '0') > 31) {
//        printf("%d\n", (*(s - 1) - '0') * 10 + *s - '0');
        return ERROR_CANT_BE_TIME; }
    ++s;
//    printf("%c", *s);
    if (*s != ':') return WRONG_FORMAT;
    ++s;
//    printf("%c", *s);
    if (!isdigit(*s++) || !isdigit(*s) || ((*(s - 1) - '0') * 10 + *s - '0') > 12) return ERROR_CANT_BE_TIME;
    ++s;
//    printf("%c", *s);
    if (*s != ':') return WRONG_FORMAT;
    ++s;
//    printf("%c", *s);
    if (!isdigit(*s++) || !isdigit(*s++) ||!isdigit(*s++) || !isdigit(*s) || ((((*(s - 3) - '0') * 10 + *(s - 2) - '0') * 10 + *(s - 1) - '0') * 10 + *s - '0') > 2024) {
        return ERROR_CANT_BE_TIME; }
//    printf("%d\n", ((((*(s - 3) - '0') * 10 + *(s - 2) - '0') * 10 + *(s - 1) - '0') * 10 + *s - '0'));

    if (*++s != ' ') return WRONG_FORMAT;
    ++s;
    if (!isdigit(*s++) || !isdigit(*s) || ((*(s - 1) - '0') * 10 + *s - '0') > 24) return ERROR_CANT_BE_TIME;
    if (*++s != ':') return WRONG_FORMAT;
    ++s;
    if (!isdigit(*s++) || !isdigit(*s) || ((*(s - 1) - '0') * 10 + *s - '0') > 60) return ERROR_CANT_BE_TIME;
    if (*++s != ':') return WRONG_FORMAT;
    ++s;
    if (!isdigit(*s++) || !isdigit(*s) || ((*(s - 1) - '0') * 10 + *s - '0') > 60) return ERROR_CANT_BE_TIME;
    if (*++s != '\0') return WRONG_FORMAT;
    return SUCCESS;
}

unsigned int Atou(const char* s) {
    while (*s == ' ') s++;
    unsigned int res = 0;
    while (*s != '\0') {
        res = (res * 10) + (*s - '0');
        s++;
    }
    return res;
}

struct String {
    int len;
    char* str;
};

int toStr(struct String *newStr, char * ptr) {
    newStr->len = 0;
    char* s = ptr;
//    printf("string: <%s>\n", ptr);
//    printf("string: <%s>\n", s);
    while (*s) {
        s++;
        newStr->len++;
    }
//    printf("len: <%d>\n", newStr->len);
    newStr->str = (char*) malloc((newStr->len + 1) * sizeof(char));
    if (newStr->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < newStr->len; ++i) {
        newStr->str[i] = ptr[i];
    }
    newStr->str[newStr->len] = '\0';
    return SUCCESS;
}

void StrClear(struct String *str) {
    free(str->str);
    str->len = 0;
}

int StrCmp(struct String str1, struct String str2) {
    if (str1.len != str2.len) {
//        printf("%d  %d\n", str1.len, str2.len);
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
    new->str = (char*) malloc((new->len + 1) * sizeof(char));
    if (new->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < new->len; i++) {
        new->str[i] = old.str[i];
    }
    new->str[new->len] = '\0';
    return SUCCESS;
}

int StrCopyToNew (struct String *new, struct String old) {
    new->len = old.len;
    new->str = (char*) malloc((new->len + 1) * sizeof(char));
    if (new->str == NULL) {
        printf("Memory allocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    for (int i = 0; i < new->len; i++) {
        new->str[i] = old.str[i];
    }
    new->str[new->len] = '\0';
    return SUCCESS;
}

int StrConc(struct String * str1, struct String str2) {
    char* ptr = (char *) realloc(str1->str, (str1->len + str2.len + 1) * sizeof(char));
    if (ptr == NULL) {
        printf("Memory reallocation error.\n");
        return MEMORY_ALLOCATION_ERROR;
    }
    str1->str = ptr;
    for (int i = 0; i < str2.len; i++) {
        str1->str[i + str1->len] = str2.str[i];
    }
    str1->len += str2.len;
    str1->str[str1->len] = '\0';
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

void printAddress(struct Address adr) {
    printf("Address: %s, %s, %u, %s, %u, ",
           adr.town.str, adr.street.str, adr.house, adr.corp.str, adr.flat);
    for (int i = 0; i < 6; i++) {
        printf("%d", adr.ind[i]);
    }
    printf("\n");
}

int newAddress(struct Address* new, char* addr) {
    char* ptrA = addr;
    char* lexeme = (char*) malloc( sizeof(char));
    if (lexeme == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    int ind_of_data = 0;
    int ind_in_lexeme = 0;
    int first = 1;

    while (*ptrA) {
        if (first) {
            first = 0;
        } else {
            ++ptrA;
        }

        if (*ptrA == '\0' && ind_of_data != 5) {
            return SUCCESS;
        }
        if (*ptrA != ' ' && *ptrA != '\n' && *ptrA != '\0') {
            printf("%c ",*ptrA);
            lexeme[ind_in_lexeme++] = *ptrA;
            if (ind_in_lexeme >= sizeof(lexeme)) {
                char *ptr;
                ptr = (char*) realloc(lexeme, (2 * (int)sizeof(lexeme) + 1) * sizeof(char));
                if (ptr == NULL) {
                    free(lexeme);
                    return MEMORY_ALLOCATION_ERROR;
                }
                lexeme = ptr;
            }
        } else {
            if (ind_in_lexeme == 0) {
                continue;
            }
            lexeme[ind_in_lexeme] = '\0';
//            printf("code: %d\n", ind_in_lexeme < sizeof(lexeme));
            printf("|%s|\n", lexeme);
            int code;
            printf("%d\n", ind_of_data);
            switch (ind_of_data) {
                case 0:
                    if ((code = is_str(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    toStr(&new->town, lexeme);
                    break;
                case 1:
                    if ((code = is_str(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    toStr(&new->street, lexeme);
                    break;
                case 2:
                    if ((code = is_number(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    new->house = Atou(lexeme);
                    break;
                case 3:
                    for (int i = 0; i < ind_in_lexeme; ++i) {
                        if (!isalnum(lexeme[i])) {
                            free(lexeme);
                            return WRONG_FORMAT;
                        }
                    }
                    toStr(&new->corp, lexeme);
                    break;
                case 4:
                    if ((code = is_number(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    new->flat = Atou(lexeme);
                    break;
                case 5:
                    if ((code = is_number(lexeme)) != SUCCESS || ind_in_lexeme != 6) {
                        free(lexeme);
                        return code;
                    }
                    for (int i = 0; i < 6; ++i) {
                        new->ind[i] = lexeme[i] - '0';
//                        printf("|%d|\n", new->ind[i]);
                    }
                    break;
                default:
                    printf("Something went wrong!\n");
                    break;
            }
            ind_of_data++;
            ind_in_lexeme = 0;
        }
    }
    free(lexeme);
    if (ind_of_data != 6) {
        printf("Wrong number of arguments.");
        return ERROR_WRONG_NUMB_OF_ARGS;
    }

    return SUCCESS;
}


void AddressClear(struct Address *adr) {
    StrClear(&adr->town);
    StrClear(&adr->street);
    StrClear(&adr->corp);
    adr->house = 0;
    adr->flat = 0;
    for(int i = 0; i < 6; i++) {
        adr->ind[i] = 0;
    }
    free(adr);
}

struct Mail {
    struct Address address;
    float weight;
    struct String postInd;
    struct String creature;
    struct String handing;
};

void printMail(struct Mail *mail) {
    printf("Mail:\n");
    printAddress(mail->address);
    printf("Weight: %f\n", mail->weight);
    printf("Post index: %s\n", mail->postInd.str);
    printf("Time of creature: %s\n", mail->creature.str);
    printf("Time of handling: %s\n", mail->handing.str);
}

int newMail(struct Mail* new) {
    char cur = '\0';
    char *lexeme = (char*) malloc(2 * sizeof(char));
//    if (lexeme == NULL) {
//        return MEMORY_ALLOCATION_ERROR;
//    }
    int lexeme_size = 1;
    int ind_in_lexeme = 0;
    int ind_of_data = 0;

    printf("Enter the receiver's address:\n");
    while (1) {
        cur = (char)getchar();
//        printf("/%c/\n", cur);
//        printf("G%dG\n", ind_of_data);
        if (ind_of_data == 5) {
            break;
        }
        if (cur != '\n') {
            printf("/%c/ ", cur);
            lexeme[ind_in_lexeme++] = cur;

            if (ind_in_lexeme >= lexeme_size) {
                lexeme_size *= 2;
                char *ptr;
                ptr = (char*) realloc(lexeme,  lexeme_size * sizeof(char));
                if (ptr == NULL) {
                    free(lexeme);
                    return MEMORY_ALLOCATION_ERROR;
                }
                lexeme = ptr;
            }
        } else {
            if (!ind_in_lexeme) {
                continue;
            }
            lexeme[ind_in_lexeme] = '\0';
            printf("|%s|\n", lexeme);
            printf("%d\n", ind_in_lexeme);
            int code;
//            printf("G%dG\n", ind_of_data);
            switch (ind_of_data) {
                case 0:
                    if ((code = newAddress(&new->address, lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    printf("Enter the package weight:\n");
                    break;
                case 1:
                    if ((code = is_float(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    new->weight = atof(lexeme);
                    printf("Enter the post index:\n");
                    break;
                case 2:
                    if (ind_in_lexeme != 14) {
                        free(lexeme);
                        return WRONG_SIZE_OF_INDEX;
                    }
                    for (int i = 0; i < 14; ++i) {
                        if (!isdigit(lexeme[i])) {
                            free(lexeme);
                            return ERROR_NOT_NUMBER;
                        }
                    }
                    toStr(&new->postInd, lexeme);
                    printf("Enter the time of creation(in the format - DD:MM:YYYY hh:mm:ss):\n");
                    break;
                case 3:
                    if ((code = is_time(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    toStr(&new->creature, lexeme);
                    printf("Enter the time of delivery(in the format - DD:MM:YYYY hh:mm:ss):\n");
                    break;
                case 4:
                    if ((code = is_time(lexeme)) != SUCCESS) {
                        free(lexeme);
                        return code;
                    }
                    toStr(&new->handing, lexeme);
                    printf("Press Enter, please.\n");
                    break;
                default:
                    printf("Something went wrong!\n");
                    break;
            }
            ind_of_data++;
            ind_in_lexeme = 0;
        }
    }
    free(lexeme);
//    if (ind_of_data != 5) {
//        printf("Wrong number of arguments.");
//        return ERROR_WRONG_NUMB_OF_ARGS;
//    }

    return SUCCESS;
}

struct Post {
    struct Address *postOffice;
    struct Mail **mails;
    int len;
};

int newPost(struct Post* new, char* address) {
    new->len = 0;
    new->mails = NULL;
    new->postOffice = (struct Address*) malloc(sizeof(struct Address));
    if (new->postOffice == NULL) {
        return MEMORY_ALLOCATION_ERROR;
    }
    newAddress(new->postOffice, address);
//    printf("Pozda\n");
    return SUCCESS;
}

int cmpMails(const void* y1, const void* y2) {
    struct Mail* x1 = (struct Mail*)y1;
    struct Mail* x2 = (struct Mail*)y2;
    for (int i =0; i < 6; ++i) {
        if (x1->address.ind[i] != x2->address.ind[i]) {
            return x1->address.ind[i] > x2->address.ind[i];
        }
    }
    return StrCmp(x1->postInd, x2->postInd);
}

int addMail(struct Post * office, struct Mail* new) {
    static int cnt;
    struct Mail ** ptrM = office->mails;

    if (!office->len) {
        office->mails = (struct Mail**) malloc(sizeof(struct Mail*));
        if (office->mails == NULL) {
            return MEMORY_ALLOCATION_ERROR;
        }
        cnt = 1;
    } else {
        if (office->len + 1 >= cnt) {
            cnt *= 2;
            struct Mail **ptr;
            ptr = (struct Mail**) realloc(office->mails, cnt * sizeof(struct Mail*));
            if (ptr == NULL) {
                free(office->mails);
                return MEMORY_ALLOCATION_ERROR;
            }
            office->mails = ptr;
        }
    }
//    printf("%d  %d\n", office->len, cnt);
    office->mails[office->len] = new;
    office->len++;
    qsort(office->mails, office->len, sizeof(struct Mail), cmpMails);
    return SUCCESS;
}

int delLastMail(struct Post * office) {
    if (office->len == 0) {
        printf("There is no Mails, you can not delete.\n");
    } else {
        office->len--;
    }
    return SUCCESS;
}

int cmpTime(const void* y1, const void* y2) {
    struct Mail* x1 = (struct Mail*)y1;
    struct Mail* x2 = (struct Mail*)y2;
    for (int i = 6; i <= 9; ++i) {
        if (x1->creature.str[i] != x2->creature.str[i]) {
            return x1->creature.str[i] < x2->creature.str[i];
        }
    }
    for (int i = 3; i <= 4; ++i) {
        if (x1->creature.str[i] != x2->creature.str[i]) {
            return x1->creature.str[i] < x2->creature.str[i];
        }
    }
    for (int i = 0; i <= 1; ++i) {
        if (x1->creature.str[i] != x2->creature.str[i]) {
            return x1->creature.str[i] < x2->creature.str[i];
        }
    }
    for (int i = 11; i < x1->creature.len; ++i) {
        if (x1->creature.str[i] != x2->creature.str[i]) {
            return x1->creature.str[i] < x2->creature.str[i];
        }
    }
    return StrCmp(x1->postInd, x2->postInd);
}

int allDelivered(struct Post * office) {
    char date[20];

    time_t rawtime;
    struct tm * timeinfo;

    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    strftime (date,20,"%d:%m:%Y %X",timeinfo);
    printf("data: |%s|", date);

    struct String full_date;

    toStr(&full_date, date);
//    printf("|%s|\n", full_date.str);

    qsort(office->mails, office->len, sizeof(struct Mail), cmpTime);
    int cnt = 1;
    for (int i = 0; i < office->len; ++i) {
        if (StrCmp(full_date, office->mails[i]->handing) >= 0) {
            printf("%d. ", cnt);
            printMail(office->mails[i]);
            printf("\n");
        }
    }

    return SUCCESS;
}

int CheckCommand(char* command) {
    char c = '1';
    char st[2] = "1\0";
    for (int i = 1; i <= 4; ++i) {
        st[0] = c;
        if (strcmp(command, st) == 0) {
            return i;
        }
        c++;
    }
    return -1;
}


int main(int argc, char* argv[]) {
//    char *test = "weekend";
//    char *test2 = "weekend";
//    struct String str;
//    toStr(&str, test);
//    struct String strnew;
//    toStr(&strnew, test2);
//    StrCopy(&strnew, str);
//    printf("%d\n", StrEq(strnew, str));
//    StrConc(&strnew, strnew);
//    for (int i = 0; i < strnew.len; i++) {
//        printf("%c ", strnew.str[i]);
//    }

    char* addres = {"hui zalupa 52 aa 12 123456"};
    struct Post head;

    newPost(&head, addres);
    printAddress(*head.postOffice);
    printf("Possible commands:\n"
           "1 - Insert new Mail.\n"
           "2 - Delete last Mail(newest).\n"
           "3 - All delivered Mails\n"
           "4 - Exit\n\n");

    int flag = 1;
    while (flag) {
        char command[256];
        printf("Enter command:\n");
        scanf("%s", command);
        int com = CheckCommand(command);
        printf("Command: %d\n", com);
        switch (com) {
            case 1: {
                struct Mail new;
                newMail(&new);
                addMail(&head, &new);
                break;
            }
            case 2: {
                delLastMail(&head);
                break;
            }
            case 3: {
                allDelivered(&head);
                break;
            }
            case 4: {
                flag = 0;
                break;
            }
            default: {
                printf("Invalid Command.\n");
                break;
            }
        }
    }
    if (head.mails != NULL) {
        for (int i = 0; i < (int)sizeof(head.mails); i++) {
            AddressClear(&head.mails[i]->address);
            head.mails[i]->weight = 0;
            StrClear(&head.mails[i]->postInd);
            StrClear(&head.mails[i]->creature);
            StrClear(&head.mails[i]->handing);
        }
        free(head.mails);
    }
    AddressClear(head.postOffice);
}