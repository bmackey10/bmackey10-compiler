#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"

typedef enum {
	EXPR_ADD,
	EXPR_SUB,
	EXPR_MUL,
	EXPR_DIV
	/* many more kinds of exprs to add here */
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int literal_value;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
/* 
calloc()
memset(e, null, sizeof(struct expr))
*/
struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
/* 
struct expr *e = expr_create(EXPR_INTEGER_LITERAL, null, null)
e->literal_value = c;
return e;
*/
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );
// expr_add(EXPR_ADD, expr_create_integer(5), expr_create_integer(10))
void expr_print( struct expr *e );

#endif
