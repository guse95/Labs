#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {
    SUCCESS,
    ERROR_WRONG_NUMBER_OF_ARGS,
    ERROR_CREATING_TABLE,
    OPENING_ERROR,
    ERROR_CHAIN_CREATING,
    ERROR_RESIZING_TABLE,
    MEMORY_ALLOCATION_ERROR
};

struct Chain {
    char *def_name;
    char *value;
    struct Chain *next;
};

int toHash(char *def_name, int HashSize) {
    int res = 0, base = 62;;
    char *ptr = def_name;
    while (*ptr) {
        if (isdigit(*ptr)) {
            res = (res * base + *ptr - '0');
        }
        else {
            res = (res * base + (((*ptr >= 'A') && (*ptr <= 'Z')) ? (*ptr - 'A' + 10) : (*ptr - 'a' + 10 + 26)));
        }
        res %= HashSize;
        ptr++;
    }
    return res;
}

int CheckCollisions(struct Chain ***HashTable, int HashSize) {
    int cur_chain = 0, max_chain = 0, min_chain = 10000;
    for (int i = 0; i < HashSize; i++) {
        if ((*HashTable)[i] != NULL) {
            struct Chain *current = (*HashTable)[i];
            cur_chain = 0;
            while (current != NULL) {
                cur_chain++;
                current = current->next;
            }
            if (cur_chain == 1) {
                cur_chain = 0;
            }

            if (cur_chain > max_chain) {
                max_chain = cur_chain;
            }
            if (cur_chain < min_chain && cur_chain != 0) {
                min_chain = cur_chain;
            }
        }
    }
    if (max_chain == 0) {
        return 0;
    }
    if ((max_chain / 2) >= min_chain) {
        return 1;
    }
    return 0;
}

struct Chain ** newTable(int HashSize) {
    struct Chain **HashTable = (struct Chain**)calloc(HashSize, sizeof(struct Chain*));
    if (HashTable == NULL) {
        return NULL;
    }
    return HashTable;
}

struct Chain *newChain(char *def_name, char *value, int len) {
    struct Chain* cur_chain = malloc(sizeof(struct Chain));
    if (cur_chain == NULL) {
        return NULL;
    }
    cur_chain->def_name = (char *)malloc(len);
    if (cur_chain->def_name == NULL) {
        free(cur_chain);
        return NULL;
    }
    strcpy(cur_chain->def_name, def_name);
    free(def_name);
    cur_chain->value = (char *)malloc(len);
    if (cur_chain->value == NULL) {
        free(cur_chain->def_name);
        free(cur_chain);
        return NULL;
    }
    strcpy(cur_chain->value, value);
    free(value);
    cur_chain->next = NULL;
    return cur_chain;
}

void CopyTable(struct Chain *** HashTable, struct Chain *** p, int HashSize) {
    struct Chain *tmp;
    for (int j = 0; j < (HashSize - 100); ++j) {
        if ((*HashTable)[j] != NULL) {
            tmp = (*HashTable)[j];
            while (tmp != NULL) {
                int hash = toHash(tmp->def_name, HashSize) % HashSize;
                if ((*p)[hash] == NULL) {
                    (*p)[hash] = tmp;

                    tmp = (*p)[hash]->next;
                    (*p)[hash]->next = NULL;
                } else {
                    struct Chain *pointer = (*p)[hash];
                    while (pointer->next != NULL) {
                        pointer = pointer->next;
                    }
                    pointer->next = tmp;
                    tmp = pointer->next->next;
                    pointer->next->next = NULL;
                }
            }
        }
    }
    free(*HashTable);
}

void PrintTable(struct Chain *** HashTable, int HashSize) {
    for (int i = 0; i < HashSize; i++) {
        if ((*HashTable)[i] != NULL) {
            printf("%d | Dir: <%s> | Val: <%s>\n", i, (*HashTable)[i]->def_name, (*HashTable)[i]->value);
            struct Chain *ptr = (*HashTable)[i]->next;
            while (ptr != NULL) {
                printf("%d next | Dir: <%s> | Val: <%s>\n", i, ptr->def_name, ptr->value);
                ptr = ptr->next;
            }
        }
    }
}

void InsertChain (struct Chain *** HashTable, int hash, struct Chain *newChain) {
    if ((*HashTable)[hash] == NULL) {
        (*HashTable)[hash] = newChain;
    } else {
        struct Chain *ptr = (*HashTable)[hash];
        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = newChain;
    }
}

void ClearTable(struct Chain ***HashTable, int HashSize) {
    struct Chain *cur;
    struct Chain *prom;
    for (int i = 0; i < HashSize; i++) {
        if ((*HashTable)[i] != NULL) {
            cur = (*HashTable)[i];
            while (cur != NULL) {
                prom = cur;
                cur = cur->next;
                free(prom->def_name);
                free(prom->value);
                free(prom);
            }
            (*HashTable)[i] = NULL;
        }
    }
    free(*HashTable);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("You forgot to point the file.\n");
        return ERROR_WRONG_NUMBER_OF_ARGS;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file <%s>.\n", argv[1]);
        return OPENING_ERROR;
    }

    FILE *out = fopen("D:\\changed.txt", "w");
    if (out == NULL) {
        printf("Error opening output file.\n");
        return OPENING_ERROR;
    }

    int HashSize = 128;
    struct Chain **HashTable = newTable(HashSize);
    if (HashTable == NULL) {
        printf("Error creating Hashtable.\n");
        return ERROR_CREATING_TABLE;
    }


    char line[BUFSIZ];
    while (1) {
        if (fgets(line, sizeof(line), file) == NULL) {
            break;
        }
        int len = (int)strlen(line);
        if (len > 11) {
            char first_word[8];
            for (int i = 0; i < 7; ++i) {
                first_word[i] = line[i];
            }
            first_word[7] = '\0';
            if (strcmp(first_word, "#define") == 0) {
                fprintf(out, "%s", line);
                char *def_name = (char *)malloc(len);
                char *value = (char *)malloc(len);
                if (def_name == NULL || value == NULL) {
                    ClearTable(&HashTable, HashSize);
                    fclose(file);
                    fclose(out);
                    printf("Memory allocation error.\n");
                    return MEMORY_ALLOCATION_ERROR;
                }
                int i = 8, ind = 0;
                while (line[i] != ' ') {
                    def_name[ind] = line[i];
                    i++;
                    ind++;
                }
                def_name[ind] = '\0';
                ind = 0;
                i++;
                while (line[i] != '\n') {
                    value[ind] = line[i];
                    i++;
                    ind++;
                }
                value[ind] = '\0';

                int hash = toHash(def_name, HashSize) % HashSize;
                struct Chain *cur_chain = newChain(def_name, value, len);
                if (cur_chain == NULL) {
                    ClearTable(&HashTable, HashSize);
                    fclose(file);
                    fclose(out);
                    printf("Error chain creating.\n");
                    return ERROR_CHAIN_CREATING;
                }

                InsertChain(&HashTable, hash, cur_chain);

                if (CheckCollisions(&HashTable, HashSize)) {
                    HashSize += 100;
                    struct Chain **ptr = newTable(HashSize);
                    if (ptr == NULL) {
                        ClearTable(&HashTable, HashSize);
                        fclose(file);
                        fclose(out);
                        printf("Error resizing table.\n");
                        return ERROR_RESIZING_TABLE;
                    }
                    CopyTable(&HashTable, &ptr, HashSize);
                    HashTable = ptr;
//                    printf("Size of table changed.\n");
                }
            } else {
                break;
            }
        } else if (line[0] == '\n') {
            fprintf(out, "%s", line);
        } else {
            break;
        }
    }
//    PrintTable(&HashTable, HashSize);

    do {
        char buf[BUFSIZ];
        char file_buf[BUFSIZ];
        int ind = 0, ind_file = 0;
        for (int i = 0; i < strlen(line) + 1; i++) {
            if (line[i] == '\n' || line[i] == '\0' || line[i] == ' ') {
                buf[ind] = '\0';
                int hash = toHash(buf, HashSize) % HashSize;
//                printf("hash: %d word: <%s>\n", hash, buf);
                ind = 0;

                if (HashTable[hash] != NULL) {
                    struct Chain *ptr = HashTable[hash];
                    while (ptr != NULL) {
                        if (strcmp(ptr->def_name, buf) == 0) {
                            char *val = ptr->value;
                            while (*val != '\0') {
                                file_buf[ind_file] = *val;
                                ind_file++;
                                val++;
                            }
                            file_buf[ind_file] = ' ';
                            ind_file++;
                        }
                        ptr = ptr->next;
                    }
                } else {
                    for (int j = 0; j < strlen(buf); j++) {
                        file_buf[ind_file] = buf[j];
                        ind_file++;
                    }
                    file_buf[ind_file] = ' ';
                    ind_file++;
                }
            } else {
                buf[ind] = line[i];
                ind++;
            }
            if (line[i] == '\n' || line[i] == '\0') {
                file_buf[ind_file] = '\n';
                ind_file++;
                file_buf[ind_file] = '\0';
                fprintf(out, "%s", file_buf);
                break;
            }
        }
    } while (fgets(line, sizeof(line), file) != NULL);


    ClearTable(&HashTable, HashSize);
    fclose(file);
    fclose(out);

    printf("\nSUCCESS\n");
    return 0;
}