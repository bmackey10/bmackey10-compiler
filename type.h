#ifndef TYPE_H
#define TYPE_H

#include "param_list.h"

typedef enum {
	TYPE_VOID,
	TYPE_BOOLEAN,
	TYPE_CHARACTER,
	TYPE_INTEGER,
	TYPE_STRING,
	TYPE_ARRAY,
	TYPE_FUNCTION,
} type_t;

struct type {
	type_t kind;
	struct param_list *params;
	struct type *subtype;
};

/* 
x: integer; --> kind = TYPE_INTEGER;
a; array [5] booleans --> 
	kind: array, subaray: pointer to type struct (next line), param_list: 0
	kind: boolean, subtype = 0, param_list = 0
*/

struct type * type_create( type_t kind, struct type *subtype, struct param_list *params );
void          type_print( struct type *t );

#endif
