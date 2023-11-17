#include "scope.h"

int resolve_error = 0;

void scope_enter() {

    struct scope *curr = malloc(sizeof(struct scope));
    curr->table = hash_table_create(0,0);
    curr->next = top;
    top = curr;

}

void scope_exit() {

    struct scope *exit = top;
    top = top->next;
    hash_table_delete(exit->table);
    free(exit);

}

void scope_bind( const char *name, struct symbol *s ) {

    if (s->type->kind == TYPE_FUNCTION) {
        struct symbol *lookup = hash_table_lookup(top->table, name);

        if (lookup && lookup->code == 1) {
            printf("ERROR: %s is already defined in this scope.\n", s->name);
            resolve_error = 1;
        } else if (lookup && lookup->code == 0) {
            if (type_equals(lookup->type, s->type) == 1) {
                printf("ERROR: %s has inconsistent parameters with original definition.\n", s->name);
                resolve_error = 1;
            } else {
                printf("%s is correctly defined.\n", s->name);
            }
            if (s->code == 1) {
                lookup->code = 1;
            }
        } else if (!lookup) {
            hash_table_insert(top->table, name, s);
            printf("%s is correctly defined.\n", s->name);
        }
    } else {
        int response = hash_table_insert(top->table, name, s);
        if (!response) {
            printf("ERROR: %s is already defined in this scope.\n", s->name);
            resolve_error = 1;
        } else {
            printf("%s is correctly defined.\n", s->name);
        }
    }
    
}

struct symbol * scope_lookup( const char *name ) {

    struct scope *curr_scope = top;

    while (curr_scope) {
        struct symbol *s = hash_table_lookup(curr_scope->table, name);
        if (s) {
            if (s->kind == SYMBOL_GLOBAL) {
                printf("%s resolves to global %s.\n", name, s->name);
            } else if (s->kind == SYMBOL_LOCAL) {
                printf("%s resolves to local %d.\n", name, s->which);
            } else if (s->kind == SYMBOL_PARAM) {
                printf("%s resolves to parameter %d.\n", name, s->which);
            }
            return s;
        }
        curr_scope = curr_scope->next;
    }

    printf("ERROR: %s could not be resolved.\n", name);
    resolve_error = 1;

    return NULL;

}