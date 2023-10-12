#ifndef DECL_H
#define DECL_H

#include "type.h"
#include "stmt.h"
#include "expr.h"
#include <stdio.h>

struct decl {
	char *name;
	struct type *type;
	struct expr *value;
	struct stmt *code;
	struct symbol *symbol;
	struct decl *next;
};

struct decl * decl_create( char *name, struct type *type, struct expr *value, struct stmt *code, struct decl *next );
/*
goes in decl.c... 
struct decl *d = malloc(sizeof(struct decl))
d->name = name;
d->type = type; ... etc
d->next = null;
return d;
*/
void decl_print( struct decl *d, int indent );

#endif


