#ifndef SCOPE_H
#define SCOPE_H

#include "symbol.h"
#include "hash_table.h"

struct scope {
    struct hash_table* table;
    struct scope* next;
};

extern struct scope *top;

void            scope_enter();
void            scope_exit();
void            scope_bind( const char *name, struct symbol *s );
struct symbol * scope_lookup( const char *name );

#endif