#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Key 
{
    long int key;
    int to;
} Key;

typedef struct Data 
{
    char data [50];
} Data;

void Create_Table (FILE* file, Key* keys, Data* datas) 
{
    for (int i = 0; i < 11; i++) 
    {
        fscanf (file, "%ld %[^\n]\n", &keys[i].key, datas[i].data);
        keys[i].to = i;
    }
}

void Print_Table (const Key* keys, const Data* datas) 
{
    printf ("|---------------------------------------------------------|\n");
    printf ("|    Ключ   |                   Строка                    |\n");
    printf ("|---------------------------------------------------------|\n");
    for (int i = 0; i < 11; i++) 
    { 
        printf ("   %ld   |%-50s\n", keys[i].key, datas [keys [i].to].data);
        printf ("|---------------------------------------------------------|\n");
    }
}

void Reverse (Key* keys) 
{
    for (int i = 0; i < 10; i++) 
    {
        Key reverse;
        reverse = keys [i];
        keys [i] = keys [10 - i];
        keys [10 - i] = reverse;
    }
}

int Order (Key* a, Key* b) 
{
    if (a->key < b->key) return -1;
    if (a->key > b->key)return 1;
    return 0;
}

void Sort_alg (Key* keys) 
{
    int x;
    for (int i = 0; i < 11; i++) 
    {
        x = i;
        for (int j = i + 1; j < 11; j++) if (Order(&keys[x], &keys [j]) == 1) x = j;
    Key tmp = keys [x];
    keys [x] = keys [i];
    keys [i] = tmp;
    }
}

void Binary_Search (Key* keys, Data* datas) 
{
    printf ("Введите значение ключа, по которому будет происходить поиск.\n");
    Key search;
    scanf ("%ld", &search.key);
    
    int L = 0, R = 10, found = 0, m;
    while (L <= R && found == 0) 
    {
        m = (L + R) / 2;
        if (Order (&keys[m], &search) == 1) R = m - 1;
        else if (Order (&search, &keys [m]) == 1) L = m + 1;
        else
        { 
            found = 1;
        }
    }
    if (found) 
    {
        printf ("%s\n",datas [keys [m].to].data);
    }
    else printf ("Строки с таким ключом не найдено\n");
}

int Sorted (Key* keys) 
{
    for (int i = 0; i < 10; i++) 
    {
        if (Order (&keys [i + 1], &keys [i]) == -1) return 0;
    }
    return 1;
}

int Reverse_Sorted (Key* keys) 
{
    for (int i = 10; i > 0; i--) 
    {
        if (Order (&keys [i - 1], &keys [i]) == -1) return 0;
    }
    return 1;
}

void Snap (Key* keys) 
{
    for (int i = 0; i < 11; i++) 
    {
        int a = rand () % 11, b = rand () % 11;
        Key temp = keys [a];
        keys [a] = keys [b];
        keys [b] = temp;
    }
}

void sort(Key* keys) {
    if (Sorted (keys)) printf ("\nТаблица уже отсортирована.\n");
    else if (Reverse_Sorted (keys)) {
        Reverse (keys);
        printf ("\nЭлементы таблицы были в порядке, обратном к сортированному\n");
    } else Sort_alg(keys);
}

void Print_Menu () {
    printf ("\nВыберите действие:\n1) Создать таблицу.\n2) Произвести сортировку выборкой с обменом с проверкой на сортированность или реверс.\n3) Бинарный поиск по ключу.\n4) Распечатать таблицу.\n5) Реверс таблицы.\n6) Перемешать таблицу.\n0) Выйти\nДействие: ");
}
 
int main (void)
{
    Key keys [11];
    Data datas [11];
    int action;
    FILE* file = fopen ("Table", "r");
    
    Create_Table (file, keys, datas);
    printf("Изначальная:\n");
    Print_Table (keys, datas);
    sort(keys);
    printf("Отсортированная:\n");
    Print_Table (keys, datas);

    Snap(keys);

    Create_Table (file, keys, datas);
    printf("Перемешанная:\n");
    Print_Table (keys, datas);
    sort(keys);
    printf("Отсортированная:\n");
    Print_Table (keys, datas);
    
    Reverse(keys);

    Create_Table (file, keys, datas);
    printf("Перемешанная:\n");
    Print_Table (keys, datas);
    sort(keys);
    printf("Отсортированная:\n");
    Print_Table (keys, datas);

    printf("Двоичный поиск:\n");
    Binary_Search (keys, datas);
    
    fclose (file);
}