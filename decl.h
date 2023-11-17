#ifndef DECL_H
#define DECL_H

#include <stdlib.h>
#include <stdio.h>
#include "expr.h"
#include "stmt.h"

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
struct decl * decl_create_func( char *name, struct type *type, struct stmt *code );
struct decl * decl_create_list(struct decl *current, struct decl *next);
void decl_print( struct decl *d, int indent );
void decl_print_list(struct decl *d);

#endif


