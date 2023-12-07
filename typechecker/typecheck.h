#ifndef TYPECHECK_H
#define TYPECHECK_H

#include "../type.h"
#include "../param_list.h"
#include "../decl.h"
#include "../stmt.h"
#include "../scope.h"

int type_equals(struct type *a, struct type *b);
int param_type_equals(struct param_list *a, struct param_list *b);
struct type * type_copy(struct type *c);
struct param_list * param_type_copy(struct param_list *c);
void type_delete(struct type *d);
void param_type_delete(struct param_list *d);
void param_list_typecheck(char *name, struct param_list *params, struct expr *args);
struct type * array_typecheck(struct expr *arr);
struct type * array_subscript_typecheck(struct type *arr_type, struct expr *index);
struct type * expr_typecheck(struct expr *e);
void decl_typecheck(struct decl *d);
struct type * stmt_typecheck(struct stmt *s);

#endif