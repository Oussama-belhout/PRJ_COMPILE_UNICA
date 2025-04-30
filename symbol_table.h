#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 100

typedef struct Symbol {
    char *name;
    char *type;
    struct Symbol *next;
} Symbol;

typedef struct {
    Symbol *table[TABLE_SIZE];
} SymbolTable;

void init_table(SymbolTable *symTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        symTable->table[i] = NULL;
    }
}

unsigned int hash(const char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % TABLE_SIZE;
}

void insert(SymbolTable *symTable, const char *name, const char *type) {
    unsigned int index = hash(name);
    Symbol *newSymbol = (Symbol *)malloc(sizeof(Symbol));
    newSymbol->name = strdup(name);
    newSymbol->type = strdup(type);
    newSymbol->next = symTable->table[index];
    symTable->table[index] = newSymbol;
}

void update(SymbolTable *symTable, const char *name, const char *type) {
    unsigned int index = hash(name);
    Symbol *symbol = symTable->table[index];
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            free(symbol->type);
            symbol->type = strdup(type);
            return;
        }
        symbol = symbol->next;
    }
    fprintf(stderr, "Error: Symbol '%s' does not exist.\n", name);
}

char* get(SymbolTable *symTable, const char *name) {
    unsigned int index = hash(name);
    Symbol *symbol = symTable->table[index];
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            return symbol->type;
        }
        symbol = symbol->next;
    }
    fprintf(stderr, "Error: Symbol '%s' not found.\n", name);
    return NULL;
}

void remove_symbol(SymbolTable *symTable, const char *name) {
    unsigned int index = hash(name);
    Symbol *symbol = symTable->table[index];
    Symbol *prev = NULL;
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            if (prev) {
                prev->next = symbol->next;
            } else {
                symTable->table[index] = symbol->next;
            }
            free(symbol->name);
            free(symbol->type);
            free(symbol);
            return;
        }
        prev = symbol;
        symbol = symbol->next;
    }
    fprintf(stderr, "Error: Symbol '%s' does not exist.\n", name);
}

int exists(SymbolTable *symTable, const char *name) {
    unsigned int index = hash(name);
    Symbol *symbol = symTable->table[index];
    while (symbol) {
        if (strcmp(symbol->name, name) == 0) {
            return 1;
        }
        symbol = symbol->next;
    }
    return 0;
}

void print_table(SymbolTable *symTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol *symbol = symTable->table[i];
        while (symbol) {
            printf("Symbol: %s, Type: %s\n", symbol->name, symbol->type);
            symbol = symbol->next;
        }
    }
}

void free_table(SymbolTable *symTable) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Symbol *symbol = symTable->table[i];
        while (symbol) {
            Symbol *temp = symbol;
            symbol = symbol->next;
            free(temp->name);
            free(temp->type);
            free(temp);
        }
    }
}

#endif // SYMBOL_TABLE_H