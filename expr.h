#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
	EXPR_ADD,
	EXPR_SUBTRACT,
	EXPR_MULTIPLY,
	EXPR_DIVIDE,
	EXPR_OR,
	EXPR_AND,
	EXPR_NOT,
	EXPR_GREATER,
	EXPR_LESS,
	EXPR_GE,
	EXPR_LE,
	EXPR_EQUALITY,
	EXPR_INEQUALITY,
	EXPR_MOD,
	EXPR_POS,
	EXPR_NEG,
	EXPR_INC,
	EXPR_DEC,
	EXPR_INTEGER,
	EXPR_FLOAT,
	EXPR_IDENTIFIER,
	EXPR_CHAR,
	EXPR_STRING,
    EXPR_BOOL
} expr_t;

struct expr {
	/* used by all kinds of exprs */
	expr_t kind;
	struct expr *left;
	struct expr *right;

	/* used by various leaf exprs */
	const char *name;
	int integer_literal;
	double float_literal;
	bool boolean_literal;
	char char_literal;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_float_literal( double c );
struct expr * expr_create_boolean_literal( int c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );
void expr_print( struct expr *e );

#endif
