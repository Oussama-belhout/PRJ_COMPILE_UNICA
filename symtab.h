#ifndef SYMTAB_H
#define SYMTAB_H

#define TABLE_SIZE 211  // A prime number for better hashing

typedef struct Symbol {
    char *name;
    int scope_level;
    char *type ;
    struct Symbol *next;  // Linked list for stack per identifier
} Symbol;

typedef struct SymbolEntry {
    char *name;
    Symbol *stack;  // Stack of definitions
    struct SymbolEntry *next;  // For collision resolution in hash table
} SymbolEntry;

// Linked list of identifiers per scope (for cleanup)
typedef struct ScopeEntry {
    char *name;
    struct ScopeEntry *next;
} ScopeEntry;

// One scope level = linked list of declared identifiers
typedef struct ScopeStack {
    ScopeEntry *ids;
    struct ScopeStack *next;
} ScopeStack;

void symtab_init();
void symtab_free();
void enter_scope();
void exit_scope();
void insert_symbol(const char *name, const char *type);
void inject_type_symbol(const char *name, const char *new_type);
Symbol *lookup_symbol(const char *name);
void print_table_symbol();
int get_current_scope();
void print_symbol_table();



#endif
