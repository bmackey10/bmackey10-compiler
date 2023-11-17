#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"
#include "expr.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_FLOAT,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
	TYPE_ERROR
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *expr;
	int contains_ident;
};

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
struct type * type_function_create( type_t kind, struct type *subtype, struct param_list *params );
struct type * type_array_create( type_t kind, struct expr *expr, struct type *subtype );
void type_print( struct type *t );

#endif
