#include <stdio.h>
#include "symbol_table.h"

extern int printd(int i);

int main() {
    SymbolTable symTable;
    init_table(&symTable);

    // Insert symbols
    insert(&symTable, "i", "int");
    insert(&symTable, "x", "int");

    // Update symbol
    update(&symTable, "x", "float");

    // Retrieve and print symbol type
    printf("Type of 'i': %s\n", get(&symTable, "i"));
    printf("Type of 'x': %s\n", get(&symTable, "x"));

    // Check if symbol exists
    if (exists(&symTable, "i")) {
        printf("'i' exists in the symbol table.\n");
    }

    // Remove symbol
    remove_symbol(&symTable, "i");

    // Print all symbols
    print_table(&symTable);

    // Free the symbol table
    free_table(&symTable);

    int i, x;
    for (i = 0; i < 10; i = i + 1) {
        if (i == 5) break;
    }
    printd(i);
    return (i + 3);
}