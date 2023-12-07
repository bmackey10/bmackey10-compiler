#ifndef CODEGEN_H
#define CODEGEN_H

#include "../decl.h"
#include "../typechecker/typecheck.h"

int label_create();
const char * label_name(int label);
char * string_label_name();

int scratch_alloc();
void scratch_free(int r);
const char * scratch_name(int r);

void pre_func_codegen(struct decl *d);
void post_func_codegen(struct decl *d);
void decl_codegen(struct decl *d);

void cmp_codegen(struct expr* e, char *type);
void func_call_codegen(struct expr* e);
void expr_codegen(struct expr *e);

void stmt_codegen(struct stmt *s, struct decl *d);
const char * symbol_codegen(struct symbol *s);

#endif