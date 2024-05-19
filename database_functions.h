#include "database_struct.h"
#include <map>

using namespace std;

void generateDatabase(ComputerDatabase* db) {
    db->count = 0;

    // Массивы для генерации случайных значений
    const char* processor_types[] = { "Intel Core i5", "Intel Core i7", "AMD Ryzen 5", "Intel Celeron" };
    const char* video_types[] = { "Integrated", "Dedicated", "PCIe" };
    const char* hdd_types[] = { "SSD", "HDD" };
    const char* os_types[] = { "Windows 10", "Windows 11", "Ubuntu", "macOS" };
    const char* owners[] = { "John Doe", "Jane Smith", "Michael Johnson", "Emily Brown", "Robert Davis" };

    // Генерация 20 записей
    for (int i = 0; i < 20; i++) {
        Computer pc;

        // Владелец
        int owner_index = rand() % 5;
        strcpy(pc.owner, owners[owner_index]);

        // Процессор
        int processor_count = rand() % 2 + 1;
        pc.processor.count = processor_count;
        int processor_type_index = rand() % 4;
        strcpy(pc.processor.type, processor_types[processor_type_index]);

        // ОЗУ
        pc.ram = (rand() % 8 + 1) * 4; // От 4 ГБ до 32 ГБ

        // Видеокарта
        int video_type_index = rand() % 3;
        strcpy(pc.video.type, video_types[video_type_index]);
        pc.video.memory = (rand() % 8 + 1); // От 1 ГБ до 8 ГБ

        // Жесткие диски
        int hdd_count = rand() % 3 + 1;
        pc.hdd.count = hdd_count;
        int hdd_type_index = rand() % 2;
        strcpy(pc.hdd.type, hdd_types[hdd_type_index]);
        pc.hdd.capacity = (rand() % 4 + 1); // От 1 ТБ до 4 ТБ

        // Контроллеры и периферийные устройства
        pc.num_controllers = rand() % 5;
        pc.num_peripherals = rand() % 10;

        // Операционная система
        int os_type_index = rand() % (sizeof(os_types) / sizeof(os_types[0]));
        strcpy(pc.os, os_types[os_type_index]);

        if (db->count < MAX_COMPUTERS) {
            db->computers[db->count++] = pc;
        }
    }
}



void printDatabase(const ComputerDatabase* db) {
    printf("%-20s %-20s %-10s %-20s %-10s %-10s %-10s %-10s %-20s\n",
           "Owner", "Processor", "RAM", "Video Card", "Video Memory", "HDD Type", "HDD Count", "HDD Capacity", "OS");
    for (int i = 0; i < db->count; i++) {
        const Computer* pc = &db->computers[i];
        printf("%-20s %-20s %-10dGB %-20s %-10dMB %-10s %-10d %-10dGB %-20s\n",
               pc->owner, pc->processor.type, pc->ram, pc->video.type, pc->video.memory,
               pc->hdd.type, pc->hdd.count, pc->hdd.capacity, pc->os);
    }
}

void printOwnersOfPoorComputer(const ComputerDatabase* db, int p) {

    map<string, int> names;

    for (int i = 0; i < db->count; ++i) {
        const Computer* pc = &db->computers[i];

        //=================================================

        bool found = false;
        if (strcmp("Intel Celeron", pc->processor.type) == 0 or pc->video.memory < 6 or pc->ram < 8) {
            found = true;
        }

        if (found) {
            if (!names[pc->owner]) {
                names[pc->owner] = 1;
            } else {
                ++names[pc->owner];
            }
            if (names[pc->owner] == p) {
                cout << pc->owner << '\n';
            }
        }
    }
}