
#ifndef STMT_H
#define STMT_H

#include "decl.h"

typedef enum {
	STMT_DECL,
	STMT_EXPR,
	STMT_IF_ELSE,
	STMT_FOR,
	STMT_PRINT,
	STMT_RETURN,
	STMT_BLOCK
} stmt_t;

struct stmt {
	stmt_t kind;
	struct decl *decl;
	struct expr *init_expr;
	struct expr *expr;
	struct expr *next_expr;
	struct stmt *body;
	struct stmt *else_body;
	struct stmt *next;
};

struct stmt * stmt_create( stmt_t kind, struct decl *decl, struct expr *init_expr, struct expr *expr, struct expr *next_expr, struct stmt *body, struct stmt *else_body, struct stmt *next );
/* 
body = {}
for (i = 0; i < 10; i++)
init_expr = i = 0
expr = i < 10
next_expr = i++

same process for stmt_create as for decl_create
struct stmt *s = malloc(sizeof(struct stmt)) --> struct stmt or s* in malloc
*/
void stmt_print( struct stmt *s, int indent );


#endif
