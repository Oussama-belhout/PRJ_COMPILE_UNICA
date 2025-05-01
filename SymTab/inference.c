#include <stdio.h>
#include "symtab.h"

void print_symbol(Symbol *s) {
    if (!s) {
        printf("Symbol not found.\n");
        return;
    }
    printf("Name: %s, Type: %s, Size: %d, Scope: %d\n",
           s->name, s->type, s->size, s->scope_level);
}

int main() {
    symtab_init();

    printf("== Entering global scope ==\n");
    printf("----------  current scope is : %d\n",get_current_scope());
    enter_scope();
    insert_symbol("x", "int", 4);
    insert_symbol("y", "float", 8);
    printf("Inserted x and y in global scope %d\n", get_current_scope());
    printf("----------  current scope is : %d\n",get_current_scope());

    printf("== Entering inner scope ==\n");
    enter_scope();
    printf("----------  current scope is : %d\n",get_current_scope());

    insert_symbol("x", "char", 1);  // Shadowing
    printf("----------  current scope is : %d\n",get_current_scope());

    insert_symbol("z", "double", 8);

    printf("Current 'x':\n");
    print_symbol(lookup_symbol("x"));

    printf("Current 'y':\n");
    print_symbol(lookup_symbol("y"));
    printf("----------  current scope is : %d\n",get_current_scope());

    printf("Modifying 'z'...\n");
    modify_symbol("z", "long", 8);
    print_symbol(lookup_symbol("z"));

    printf("== Exiting inner scope ==\n");
    printf("----------  current scope is : %d\n",get_current_scope());

    exit_scope();
    printf("----------  current scope is : %d\n",get_current_scope());

    printf("Now 'x' refers to:\n");
    print_symbol(lookup_symbol("x"));  // Back to global

    printf("== Exiting global scope ==\n");
    exit_scope();
    printf("----------  current scope is : %d\n",get_current_scope());

    printf("After all scopes, 'x':\n");
    print_symbol(lookup_symbol("x"));

    symtab_free();
    return 0;
}
