#ifndef EXPR_H
#define EXPR_H

#include "symbol.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
	EXPR_EXP, 
	EXPR_POS,
	EXPR_NEG,
	EXPR_INC,
	EXPR_DEC,
	EXPR_INTEGER,
	EXPR_FLOAT,
	EXPR_IDENTIFIER,
	EXPR_CHAR,
	EXPR_STRING,
    EXPR_BOOL,
	EXPR_ARR,
	EXPR_LIST,
	EXPR_FUNC,
	EXPR_ARR_SUB,
	EXPR_INDEX,
	EXPR_ASSIGN
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
	int boolean_literal;
	char char_literal;
	const char * string_literal;
	struct symbol *symbol;
};

struct expr * expr_create( expr_t kind, struct expr *left, struct expr *right );
struct expr * expr_create_name( const char *n );
struct expr * expr_create_integer_literal( int c );
struct expr * expr_create_float_literal( double c );
struct expr * expr_create_boolean_literal( char *c );
struct expr * expr_create_char_literal( char c );
struct expr * expr_create_string_literal( const char *str );
struct expr * expr_create_array_literal( struct expr *expr );
struct expr * expr_create_list( struct expr *current, struct expr *next );
struct expr * expr_create_func_call( struct expr *name, struct expr *args);
struct expr * expr_create_arr_subscript( struct expr *name, struct expr *index);
struct expr * expr_create_arr_index( struct expr *name, struct expr *index);
char parse_char(char *str_in);
char * parse_string(char *str_in);
double parse_float(char *str_in);
void expr_print_list(struct expr *e);
char* get_char(char c);
char* get_string(const char *str);
int get_rank( expr_t curr);
int get_parens(struct expr *prev, struct expr *curr);
void expr_print( struct expr *p, struct expr *e );

#endif
