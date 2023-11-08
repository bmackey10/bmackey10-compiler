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
	TYPE_FUNCTION
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
	struct expr *expr;
};

/* 
x: integer; --> kind = TYPE_INTEGER;
a; array [5] booleans --> 
	kind: array, subaray: pointer to type struct (next line), param_list: 0
	kind: boolean, subtype = 0, param_list = 0
*/

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
struct type * type_function_create( type_t kind, struct type *subtype, struct param_list *params );
struct type * type_array_create( type_t kind, struct expr *expr, struct type *subtype );
void type_print( struct type *t );
int type_check(struct type *a, struct type *b);

#endif
