#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "symtab.h"

static SymbolEntry *table[TABLE_SIZE];
static ScopeStack *scope_stack = NULL;
static int current_scope = 0;

unsigned int hash(const char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % TABLE_SIZE;
}

void symtab_init() {
    memset(table, 0, sizeof(table));
    scope_stack = NULL;
    current_scope = 0;
}
int check_declaration(const char *name) {
    Symbol *sym = lookup_symbol(name);
    return sym && sym->scope_level == current_scope;
}


void symtab_free() {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        SymbolEntry *entry = table[i];
        while (entry) {
            Symbol *sym = entry->stack;
            while (sym) {
                Symbol *tmp = sym;
                sym = sym->next;
                free(tmp->name);
                free(tmp->type);
                free(tmp);
            }
            SymbolEntry *tmp_entry = entry;
            entry = entry->next;
            free(tmp_entry->name);
            free(tmp_entry);
        }
    }

    while (scope_stack) {
        ScopeEntry *id = scope_stack->ids;
        while (id) {
            ScopeEntry *tmp = id;
            id = id->next;
            free(tmp->name);
            free(tmp);
        }
        ScopeStack *tmp_scope = scope_stack;
        scope_stack = scope_stack->next;
        free(tmp_scope);
    }
}

void enter_scope() {
    ScopeStack *new_scope = malloc(sizeof(ScopeStack));
    new_scope->ids = NULL;
    new_scope->next = scope_stack;
    scope_stack = new_scope;
    current_scope++;
}

void exit_scope() {
    if (!scope_stack) return;

    ScopeEntry *id = scope_stack->ids;
    while (id) {
        unsigned int idx = hash(id->name);
        SymbolEntry *entry = table[idx];
        while (entry && strcmp(entry->name, id->name) != 0)
            entry = entry->next;

        if (entry && entry->stack) {
            Symbol *top = entry->stack;
            entry->stack = top->next;
            free(top->name);
            free(top->type);
            free(top);
        }

        ScopeEntry *tmp = id;
        id = id->next;
        free(tmp->name);
        free(tmp);
    }

    ScopeStack *old_scope = scope_stack;
    scope_stack = scope_stack->next;
    free(old_scope);
    current_scope--;
}

void insert_symbol(const char *name, const char *type) {
    unsigned int idx = hash(name);
    SymbolEntry *entry = table[idx];

    while (entry && strcmp(entry->name, name) != 0)
        entry = entry->next;

    if (!entry) {
        entry = malloc(sizeof(SymbolEntry));
        entry->name = strdup(name);
        entry->stack = NULL;
        entry->next = table[idx];
        table[idx] = entry;
    }

    Symbol *symbol = malloc(sizeof(Symbol));
    symbol->name = strdup(name);
    symbol->type = strdup(type);
    symbol->scope_level = current_scope;
    symbol->next = entry->stack;
    entry->stack = symbol;

    ScopeEntry *id = malloc(sizeof(ScopeEntry));
    id->name = strdup(name);
    id->next = scope_stack->ids;
    scope_stack->ids = id;
}

void inject_type_symbol(const char *name, const char *type) {
    Symbol *sym = lookup_symbol(name);
    if (sym) {
        free(sym->type);
        sym->type = strdup(type);
    }
}

Symbol *lookup_symbol(const char *name) {
    unsigned int idx = hash(name);
    SymbolEntry *entry = table[idx];
    while (entry && strcmp(entry->name, name) != 0)
        entry = entry->next;
    return (entry && entry->stack) ? entry->stack : NULL;
}

int get_current_scope() {
    return current_scope;
}
void print_symbol_table() {
    printf("=== Symbol Table ===\n");
    for (int i = 0; i < TABLE_SIZE; i++) {
        SymbolEntry *entry = table[i];
        while (entry) {
            Symbol *sym = entry->stack;
            printf("Identifier: %s\n", entry->name);
            while (sym) {
                printf("  ↳ Type: %s | Scope: %d\n", sym->type ? sym->type : "(undefined)", sym->scope_level);
                sym = sym->next;
            }
            entry = entry->next;
        }
    }
    printf("====================\n");
}

/* INJECT TYPE IS THERE SO NO NEED FOR THIS
void modify_symbol(const char *name, const char *new_type, int new_size) {
    Symbol *sym = lookup_symbol(name);
    if (sym) {
        free(sym->type);
        sym->type = strdup(new_type);
        sym->size = new_size;
    }
}*/

