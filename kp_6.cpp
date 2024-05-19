#include "database_functions.h"

int main() {

    ComputerDatabase db;
    generateDatabase(&db);
    printf("Database contents:\n");
    printDatabase(&db);
    printf("\nOwners Of Poor Computers:\n");
    printOwnersOfPoorComputer(&db, 2);
    return 0;
}