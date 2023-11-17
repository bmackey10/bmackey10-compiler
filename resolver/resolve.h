#ifndef RESOLVE_H
#define RESOLVE_H

#include "../decl.h"
#include "../expr.h"
#include "../param_list.h"

void decl_resolve(struct decl* d, int which);
int stmt_resolve(struct stmt* s, int which);
void expr_resolve(struct expr* e);
void param_resolve(struct param_list *p, int which);

#endif