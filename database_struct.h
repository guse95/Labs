#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 50
#define MAX_OS_LEN 20
#define MAX_COMPUTERS 10

typedef struct {
    char type[20];
    int count;
} Processor;

typedef struct {
    char type[20];
    int memory;
} VideoCard;

typedef struct {
    char type[10];
    int count;
    int capacity;
} HardDrive;

typedef struct {
    char owner[MAX_NAME_LEN];
    Processor processor;
    int ram;
    VideoCard video;
    HardDrive hdd;
    int num_controllers;
    int num_peripherals;
    char os[MAX_OS_LEN];
} Computer;

typedef struct {
    Computer computers[MAX_COMPUTERS];
    int count;
} ComputerDatabase;